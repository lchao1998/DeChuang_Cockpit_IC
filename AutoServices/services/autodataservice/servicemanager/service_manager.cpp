#include "service_manager.hpp"
#include "appfw/application.hpp"
#include "log/log.hpp"
#include "autosocm/application.hpp"
#include "autosocm/data.hpp"
#include "socketcan_controller.hpp"

// handshake cycle time
#define HANDle_SEQUNCE_CYCLE_TIME_MS 10U
// speed interval
#define INTERVAL_SPEED 50U
#define INTERVAL_MOTORPOWERRATE 20U

namespace AutoDataService
{
    #define RECV_DATA_SIZE 32
    const char*    OUTPUT_TAG = "AutoDataService::ServiceManager";
    const string SOC_AUTO_DATA_SERVICE_MANAGER_NAME =  "ServiceManager";

    #define SET_SERVICE_NUMBER_VALUE(service, target, type) setNumberValue(service, target.type, #type)

    /**
    * @fn setNumberValue
    * @brief set shared memory data of service attribute value
    * @param const shared_ptr<type0>& service   : service object
    * @param const typ2e& target                : change data value
    * @param const char* name                   : attribute name
    * @return void
    */
    template <typename type0, typename type1>
    void setNumberValue(const shared_ptr<type0>& service, const type1& target, const char* name)
    {
        service->setAttribute(name, static_cast<uint32_t>(target));
    }

    ServiceManager::ServiceManager(void):
		Module(), 
		m_domainService(this),
		m_gaugeService(this),
		m_recv_stop(false)
		//m_handleCanDataTimer(this, HANDle_SEQUNCE_CYCLE_TIME_MS, std::bind(&ServiceManager::canDataHandler, this))
    {
			//m_handleCanDataTimer.start();
		m_funcMap.emplace(CANID_VCU_VehInfoA, std::function<void(const can_frame& frame)>(
			[this](const can_frame& frame)->void
			{
				this->process_vcu_vehinfoa(frame);
			}));

		m_funcMap.emplace(CANID_VCU_VehInfoB, std::function<void(const can_frame& frame)>(
			[this](const can_frame& frame)->void
			{
				this->process_vcu_vehinfob(frame);
			}));

		m_funcMap.emplace(CANID_VCU_MotST, std::function<void(const can_frame& frame)>(
			[this](const can_frame& frame)->void
			{
				this->process_vcu_motst(frame);
			}));

		m_funcMap.emplace(CANID_VCU_CCVS, std::function<void(const can_frame& frame)>(
			[this](const can_frame& frame)->void
			{
				this->process_vcu_ccvs(frame);
			}));
		m_funcMap.emplace(CANID_VCU_DispInfo, std::function<void(const can_frame& frame)>(
			[this](const can_frame& frame)->void
			{
				this->process_vcu_dispinfo(frame);
			}));
		m_funcMap.emplace(CANID_BCM_CPD1, std::function<void(const can_frame& frame)>(
			[this](const can_frame& frame)->void
			{
				this->process_vcu_cpd1(frame);
			}));
		m_funcMap.emplace(CANID_BCM_CPD3, std::function<void(const can_frame& frame)>(
			[this](const can_frame& frame)->void
			{
				this->process_vcu_cpd3(frame);
			}));
		m_funcMap.emplace(CANID_DCM_ConfigStatus, std::function<void(const can_frame& frame)>(
			[this](const can_frame& frame)->void
			{
				this->process_vcu_configstatus(frame);
			}));
		m_funcMap.emplace(CANID_EPS_ERROR, std::function<void(const can_frame& frame)>(
		[this](const can_frame& frame)->void
		{
			this->process_eps_error(frame);
		}));
		m_recv_thread = std::thread([this](){receive();});
    }

    ServiceManager::~ServiceManager(void)
    {
			//m_handleCanDataTimer.stop();
		m_recv_stop = true;
		 // wait func receive() return
        std::unique_lock<std::mutex> lck(m_recv_mutex);
        m_recv_condition.wait(lck);
		// notify func receive() to stop
        if (m_recv_thread.joinable())
        {
            m_recv_thread.join();
        }
    }

	void ServiceManager::receive()
	{
		std::unique_lock<std::mutex> lk(m_recv_mutex);
		can_frame recvFrame;
		while (!m_recv_stop)
		{
			if (!SocketCanController::getInstance().bInited())
			{
				//logDebug("receive(): socket can is not initalized!");

				int32_t ret = SocketCanController::getInstance().init();
				//logDebug("init socketcan %d \n", ret);
				usleep(1000);
				continue;
			}

			int ret = SocketCanController::getInstance().recv(recvFrame);
			if(ret > 0 ) {
				recvFrame.can_id = recvFrame.can_id & CAN_EFF_MASK;

				// CAN ID
				/**
				#define CANID_VCU_VehInfoA 			(0x1CFF0227 & CAN_EFF_MASK)
				#define CANID_VCU_VehInfoB			(0x10FF0327 & CAN_EFF_MASK)
				#define CANID_VCU_MotST				(0x10FF8A27 & CAN_EFF_MASK)
				#define CANID_VCU_CCVS				(0x18FEF127 & CAN_EFF_MASK)
				#define CANID_VCU_DispInfo			(0x18FF0727 & CAN_EFF_MASK)
				#define CANID_BCM_CPD1				(0x18FDCD21 & CAN_EFF_MASK)
				#define CANID_BCM_CPD3				(0x18FFD921 & CAN_EFF_MASK)
				#define CANID_DCM_ConfigStatus		(0x18FDA5EC & CAN_EFF_MASK)
				#define CANID_EPS_ERROR             (0x18FFD013 & CAN_EFF_MASK)
				*/
				if(	recvFrame.can_id == CANID_VCU_VehInfoA ||
						recvFrame.can_id == CANID_VCU_VehInfoB ||
						recvFrame.can_id == CANID_VCU_MotST ||
						recvFrame.can_id == CANID_VCU_CCVS ||
						recvFrame.can_id == CANID_VCU_DispInfo ||
						recvFrame.can_id == CANID_BCM_CPD1 ||
						recvFrame.can_id == CANID_BCM_CPD3 ||
						recvFrame.can_id == CANID_DCM_ConfigStatus ||
						recvFrame.can_id == CANID_EPS_ERROR
				  ) {
						auto itor = m_mapCanframe.find(recvFrame.can_id);
						if(itor != m_mapCanframe.end()){
								if (itor->second.can_id == recvFrame.can_id &&
										itor->second.can_dlc == recvFrame.can_dlc &&
										memcmp(itor->second.data, recvFrame.data, recvFrame.can_dlc) == 0) {
										// frames are equal
								}else {

									// special handle speed and MotorPowerRate

									// if(recvFrame.can_id == CANID_VCU_VehInfoB) {
									// 		if(memcmp(itor->second.data, recvFrame.data, 2) != 0){
									// 				if(m_speedInterval >= INTERVAL_SPEED) {
									// 					m_frameBuffer.push(recvFrame);
									// 					m_speedInterval = 0;
									// 				}
									// 		}
									// }else if(recvFrame.can_id == CANID_VCU_MotST) {
									// 		if(memcmp(itor->second.data, recvFrame.data, 2) != 0){
									// 				if(m_motorPowerRateInterval >= INTERVAL_MOTORPOWERRATE) {
									// 					m_frameBuffer.push(recvFrame);
									// 					m_motorPowerRateInterval = 0;
									// 				}
									// 		}
									// }else {
									// 		m_frameBuffer.push(recvFrame);
									// }
									m_frameBuffer.push(recvFrame);
										
								}

					}else{
						m_mapCanframe.emplace(recvFrame.can_id, recvFrame);
					}
				}
				//logDebug("recv socketcan data can id %x len %d \n", recvFrame.can_id, recvFrame.can_dlc);
			}
		}
		m_recv_condition.notify_all();
	}

	bool ServiceManager::readMsg(can_frame& frame) {
		if( m_frameBuffer.size() <= 0) return false;

		//logDebug("-----buffer size: %d ! \n", m_frameBuffer.size());  
		frame = m_frameBuffer.front();
		std::unique_lock<std::mutex> lk(m_data_mutex);
		m_frameBuffer.pop();
		return true;
	}
	void ServiceManager::canDataHandler() {
		m_speedInterval += HANDle_SEQUNCE_CYCLE_TIME_MS;
		m_motorPowerRateInterval += HANDle_SEQUNCE_CYCLE_TIME_MS;
	}
  void ServiceManager::onProcess()
  {
		if (!SocketCanController::getInstance().bInited())
		{
			//logDebug("socket init failed ! \n");
			SocketCanController::getInstance().init();
			return;
		}
		
		postTask([=] {
						can_frame frame;
						if( readMsg(frame)) 
						{	
							//logDebug("read one can frame [id:%x]-[len:%d] ! \n", frame.can_id, frame.can_dlc);
							auto itor = m_funcMap.find(frame.can_id);
							if(itor != m_funcMap.end())
							{
								itor->second(frame);

							}
						}
				}); 
    }
	void ServiceManager::process_vcu_vehinfoa(const can_frame& frame)
	{
		CanSignalVCUVehInfoA canSignalVCUVehInfoA;
		memcpy(&canSignalVCUVehInfoA, frame.data, CAN_DATA_LENGTH);

		uint16_t volt = (uint16_t)(canSignalVCUVehInfoA.signal.vehicleVoltageLow | canSignalVCUVehInfoA.signal.vehicleVoltageHigh << 8);
		volt = uint16_t(volt*0.1);
		if(volt > 1000)

		//logDebug("VCU_VehVolt: %d \n", volt);
		m_gaugeService.getService()->setAttribute("VCU_VehVolt", volt);

		int8_t soc = canSignalVCUVehInfoA.signal.vehicleSOC;
		if(soc > 100 || soc < 0) soc = 0;
		m_gaugeService.getService()->setAttribute("VCU_VehSOC", soc);

		uint16_t distance = (uint16_t)(canSignalVCUVehInfoA.signal.vehicleDistanceRangeLow | canSignalVCUVehInfoA.signal.vehicleDistanceRangeHigh << 8);
		//logDebug("VCU_DistanceRange: %d \n", distance);
		if(distance > 1000 || distance < 0) distance = 0; 
		m_gaugeService.getService()->setAttribute("VCU_DistanceRange", distance);

	}

	void ServiceManager::process_vcu_vehinfob(const can_frame& frame)
	{
		CanSignalVCUVehInfoB canSignalVCUVehInfoB;
		memcpy(&canSignalVCUVehInfoB, frame.data, CAN_DATA_LENGTH);

        uint16_t speed = (uint16_t)(canSignalVCUVehInfoB.signal.wheelBasedVehicleSpeedLow | canSignalVCUVehInfoB.signal.wheelBasedVehicleSpeedHigh << 8);
		speed = (uint16_t)(speed * 0.00390625);
		if(speed > 140 || speed < 0) speed = 0;
		//logDebug("VCU_VehSpd: %d \n", speed);
		m_gaugeService.getService()->setAttribute("VCU_VehSpd", speed);

		uint8_t shiftLevel = canSignalVCUVehInfoB.signal.vehicleShiftLevel;
		m_gaugeService.getService()->setAttribute("VCU_VehShiftLevel", shiftLevel);
		//logDebug("VCU_VehShiftLevel: %d \n", shiftLevel);
	}

	void ServiceManager::process_vcu_motst(const can_frame& frame)
	{
		CanSignalVCUMotST           canSignalVCUMotST;
        memcpy(&canSignalVCUMotST, frame.data, CAN_DATA_LENGTH);

		int8_t motorPowerRate = canSignalVCUMotST.signal.motorPowerRate;
		motorPowerRate -= 60;
		if(motorPowerRate > 100 || motorPowerRate < -60) motorPowerRate = 0;
		m_gaugeService.getService()->setAttribute("VCU_MotPwrRate", motorPowerRate);
	}

	void ServiceManager::process_vcu_ccvs(const can_frame& frame)
	{
		CanSignalVCUCCVS            canSignalVCUCCVS;
        memcpy(&canSignalVCUCCVS, frame.data, CAN_DATA_LENGTH);

		int8_t parkBrkSW = canSignalVCUCCVS.signal.parkingBrakeSwitch;
		m_gaugeService.getService()->setAttribute("VCU_ParkBrkSW", parkBrkSW);
		m_gaugeService.getService()->setAttribute("VCU_CCActive", canSignalVCUCCVS.signal.cruiseControlActive);
		m_gaugeService.getService()->setAttribute("VCU_CCEnableSW", canSignalVCUCCVS.signal.cruiseControlEnable);
	}

	void ServiceManager::process_eps_error(const can_frame& frame)
	{
		CanSignalEPSError epsError;
		memcpy(&epsError, frame.data, CAN_DATA_LENGTH);
		m_domainService.getService()->setAttribute("EPS_ErrLevel", epsError.signal.EPSErrorLevel);
	}

	void ServiceManager::process_vcu_dispinfo(const can_frame& frame)
	{
		CanSignalVCUDispInfo        canSignalVCUDispInfo;
        memcpy(&canSignalVCUDispInfo, frame.data, CAN_DATA_LENGTH);

		int8_t vehicleErrorLamp = canSignalVCUDispInfo.signal.vehicleErrorLamp;
		m_domainService.getService()->setAttribute("VCU_VehErrInd", vehicleErrorLamp);
		////logDebug("VCU_VehErrInd %d \n", vehicleErrorLamp);

		int8_t bmsErrorLamp = canSignalVCUDispInfo.signal.bmsErrorLamp;
		m_domainService.getService()->setAttribute("VCU_BMSErrInd", bmsErrorLamp);

		int8_t insulationErrorIndication = canSignalVCUDispInfo.signal.insulationErrorIndication;
		m_domainService.getService()->setAttribute("VCU_InsErrInd", insulationErrorIndication);
        
		int8_t driveReadyIndication = canSignalVCUDispInfo.signal.driveReadyIndication;
		m_domainService.getService()->setAttribute("VCU_DrvReadyInd", driveReadyIndication);

		int8_t socLowIndication = canSignalVCUDispInfo.signal.socLowIndication;
		m_domainService.getService()->setAttribute("VCU_SOCLowInd ", socLowIndication);

		int8_t battChargeErrorIndication = canSignalVCUDispInfo.signal.battChargeErrorIndication;
		m_domainService.getService()->setAttribute("VCU_BattChrgErrInd", battChargeErrorIndication);

		int8_t tcuErrorIndication = canSignalVCUDispInfo.signal.tcuErrorIndication;
		m_domainService.getService()->setAttribute("VCU_TCUErrInd", tcuErrorIndication);

		int8_t vehicleSTOPIndication = canSignalVCUDispInfo.signal.vehicleSTOPIndication;
		m_domainService.getService()->setAttribute("VCU_VehSTOPInd", vehicleSTOPIndication);
	}

	void ServiceManager::process_vcu_cpd1(const can_frame& frame)
	{
		CanSignalVCUCPD1            canSignalVCUCPD1;
        memcpy(&canSignalVCUCPD1, frame.data, CAN_DATA_LENGTH);

		int8_t positionLightStatusIndicator = canSignalVCUCPD1.signal.positionLightStatusIndicator;
		m_domainService.getService()->setAttribute("BCM_PosLgtStInd", positionLightStatusIndicator);

		int8_t frontFogStatus = canSignalVCUCPD1.signal.frontFogStatus;
		m_domainService.getService()->setAttribute("BCM_FrontFogLightSts", frontFogStatus);

		int8_t lowBeamStatus = canSignalVCUCPD1.signal.lowBeamStatus;
		m_domainService.getService()->setAttribute("BCM_LBeamSt", lowBeamStatus);

		int8_t highBeamStatus = canSignalVCUCPD1.signal.highBeamStatus;
		m_domainService.getService()->setAttribute("BCM_HBeamSt", highBeamStatus);

		int8_t rearFogStatus = canSignalVCUCPD1.signal.rearFogStatus;
		m_domainService.getService()->setAttribute("BCM_RFogLgtSt", rearFogStatus);
        
	}

	void ServiceManager::process_vcu_cpd3(const can_frame& frame)
	{
		CanSignalVCUCPD3            canSignalVCUCPD3;
        memcpy(&canSignalVCUCPD3, frame.data, CAN_DATA_LENGTH);
        
		uint16_t batteryVoltage = (uint16_t)(canSignalVCUCPD3.signal.batteryVoltageLow  | canSignalVCUCPD3.signal.batteryVoltageHigh << 6);
		//logDebug("-------batteryVoltage: %x", batteryVoltage);
		float bv = batteryVoltage * 0.001953125;
		bv += 6.0;
		if((bv - 37.0) > 0.001 || (bv - 6.0 ) < 0.001) bv = 0.0;
		m_domainService.getService()->setAttribute("BCM_U_BATT", bv);
		//logDebug("------bv: %f", bv);
		int8_t leftTurnLightCommandtoIC = canSignalVCUCPD3.signal.leftTurnLightCommandtoIC;
		//logDebug("BCM_L_TurnLgtCmdToIC: %d \n", leftTurnLightCommandtoIC);
		m_domainService.getService()->setAttribute("BCM_L_TurnLgtCmdToIC", leftTurnLightCommandtoIC);

		int8_t rightTurnLightCommandtoIC = canSignalVCUCPD3.signal.rightTurnLightCommandtoIC;
		//logDebug("BCM_R_TurnLgtCmdToIC: %d \n", rightTurnLightCommandtoIC);
		m_domainService.getService()->setAttribute("BCM_R_TurnLgtCmdToIC", rightTurnLightCommandtoIC);
	}

	void ServiceManager::process_vcu_configstatus(const can_frame& frame)
	{
		CanSignalVCUConfigStatus    canSignalVCUConfigStatus;
        memcpy(&canSignalVCUConfigStatus, frame.data, CAN_DATA_LENGTH);

		int8_t driverDoorAjarSwitch = canSignalVCUConfigStatus.signal.driverDoorAjarSwitch;
		m_domainService.getService()->setAttribute("DCM_DriverDoorAjarSW", driverDoorAjarSwitch);

		int8_t passengerDoorAjarSwitch = canSignalVCUConfigStatus.signal.passengerDoorAjarSwitch;
		m_domainService.getService()->setAttribute("DCM_PassengerDoorAjarSW", passengerDoorAjarSwitch);

		int8_t doorNotClosedInd = canSignalVCUConfigStatus.signal.doorNotClosedInd;
		m_domainService.getService()->setAttribute("DCM_DoorNotClosedInd", doorNotClosedInd);
	}
 }
