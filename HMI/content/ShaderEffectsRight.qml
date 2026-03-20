import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Timeline 1.0

ShaderEffect{
    id: vehicleSpeedDisplay
    width: 243
    height: 447
    property int speed: gaugedatamodel.gauge_VCU_VehSOC
    property real hight: 1.99
    property variant source: sourceImage
    x: 0
    y: 0

    Behavior on hight{
        SmoothedAnimation{duration: 500;velocity: -1}
    }

          vertexShader: "
          uniform highp mat4 qt_Matrix;
          attribute highp vec4 qt_Vertex;
          attribute highp vec2 qt_MultiTexCoord0;
          varying highp vec2 thePos;
          varying highp vec2 texcoord;
          void main() {
          thePos =  vec2(qt_MultiTexCoord0.x*2.0-1.0,-qt_MultiTexCoord0.y*2.0+1.0);
          texcoord=qt_MultiTexCoord0;
          gl_Position = qt_Matrix * qt_Vertex;
          }
          "

          fragmentShader: "
          varying highp vec2 thePos;
          varying highp vec2 texcoord;
          uniform sampler2D source;
          uniform float hight;
          uniform float qt_Opacity;

          vec4 outColor;

          //Rotation matrix
          mat2 rotate2d(float _angle){
          float angle = radians(_angle);
          return mat2(cos(angle),sin(angle),
                         -sin(angle),cos(angle));
           }

          //Square mask
          vec3 drawBox(vec2 pos,vec2 center,float len)
          {
              vec3 col = vec3(0.0,0.0,0.0);
              if(abs(pos.x-center.x)<len*2. && abs(pos.y-center.y)<len*1.0)
                  col = vec3(1.0,1.0,1.0);
              return col;
          }

          void main()
          {
          vec2 uv=texcoord;
          uv.y=1.0-uv.y;

          uv=uv*2.-1.;
          uv.x*=1.31/4.91;

        uv.x+=0.6;
        uv.y-=0.8;

        //Judge square mask rotation
        if(hight>=1.35){
        uv=rotate2d(-60.)*uv;
        uv.y-=hight-2.21;
        }

        else if(1.05<=hight&&hight<1.35){
        //先旋转再移动
        uv=rotate2d(-60.)*uv;
        uv.y-=-0.86;
        uv=rotate2d((1.35-hight)*200.)*uv;
        }

        else{
        uv=rotate2d(-60.)*uv;
        uv.y-=-0.25;
        uv=rotate2d(60.)*uv;

        uv.y+=1.35-hight;
        }

        //Calculation mask
        vec2 boxcenter=vec2(0.0);
        vec3 box =drawBox(uv+vec2(-0.5,-1.),boxcenter,1.);
        float d=length(box);
        vec4 color = texture2D(source, texcoord);

        gl_FragColor =vec4(color*(1.0-d))* qt_Opacity;
        }
        "
    Image {
        id: sourceImage
        source: "images/gauge3_right.png"
        visible: false
    }
    Timeline {
     id: timeline
     currentFrame: vehicleSpeedDisplay.speed
     enabled: true
     endFrame: 100
     startFrame: 0

     KeyframeGroup {
         target: vehicleSpeedDisplay
         property: "hight"
         Keyframe {
             frame: 0
             value: "-0.35"
         }

         Keyframe {
             frame: 100
             value: "1.99"
         }

     }
 }
}


