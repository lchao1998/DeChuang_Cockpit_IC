#include "configuration.hpp"
#include <string.h>
#include <stdlib.h>

namespace AppFw
{
    Configuration conf;
void Configuration::parseFromCommandline(int argc, char **argv)
{
    for(int i=0;i<argc;++i)
    {
        if(!strcmp(argv[i],"-interval"))
        {
            conf.execInterval = atoi(argv[i+1]);
        }
        else if(!strcmp(argv[i],"-threads"))
        {
            conf.maxWorkingThreads = atoi(argv[i+1]);
        }
        else if(!strcmp(argv[i],"-log"))
        {
            conf.logSeverity = atoi(argv[i+1]);
        }
    }
}

Configuration &Configuration::get()
{
    return conf;
}
}
