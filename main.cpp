/*Created by cordese
 * Binary for stress sybase runing two stores procedures in differents threads and with distinct conections
 * With configurables parameters
 */

#include <iostream>
#include "ProducerConsumer.hpp"
#include <pthread.h>
#include <fstream>
#include <cstring>
#include <zconf.h>
#include <vector>
#include <algorithm>
#include <utility>


using std::string;
using namespace std;
#define  numberThread 4


void * RunThread(void *threadarg)
{
	ProducerConsumer *my_data;
    my_data = (ProducerConsumer *) threadarg;

    try
    {
        my_data->run();
    }
    catch(...)
    {
        printf("Error in thread");
    }
    return 0;
}

void usage()
{
	printf("Usage: \n");
	printf("-I [filename] -i [file name] File name input [mandatory] \n");
	printf("-O [filename] -o [file name] File name output [mandatory] \n");
	printf("-S [order] -s [order] Order way [mandatory] \n");
	printf("-H or -h Show this help \n");
}

int main(int argc, char* argv[])
{
    pthread_attr_t attr;
    string sline;
    int index = 0;
    int sizeT = 0x800000;
    int init;
    int rc;
    int opt;
    char *fileNameInput = NULL;
    char *fileNameOutput = NULL;
    int divition = 0;
    int module = 0;
    int order = 0;


    try
    {
        while(( opt = getopt(argc, argv, "I:i:O:o:S:s:")) != -1)
        {
            switch (opt)
            {
            case 'I':
            case 'i':
                fileNameInput = optarg;
                break;
            case 'O':
            case 'o':
            	fileNameOutput = optarg;
                break;
            case 'h':
            case 'H':
                usage();
                exit(1);
                break;
            case 'S':
            case 's':
                 order = atoi(optarg);
                 break;
            case '?':
                printf("Option not recognized \n");
                usage();
                exit(1);
                break;
            }
        }
        if(fileNameOutput == NULL || fileNameInput==NULL || order == 0)
        {
            printf("Options -I or -i and -o or -O and -S or -s are mandatory \n");
            usage();
            exit(1);
        }
        std::ifstream file(fileNameInput);
        std::ifstream fileout(fileNameOutput);
        pthread_t threads[numberThread];
        std::vector<ProducerConsumer*> m_instance(numberThread);
        std::vector<string> vSubscriber;
        std::vector<int> vThread;

        init = pthread_attr_init(&attr);
        if(init != 0)
        {
            printf("Error charging attribute of thread");
            exit(1);
        }
        init = pthread_attr_setstacksize(&attr, sizeT);
        if(init != 0)
        {
            printf("Error resizing memory of thread");
            exit(1);
        }
        if(file.is_open())
        {
           while(std::getline(file, sline))
           {
               if(sline.empty())
               {
                   printf("Invalid format check the file content");
                   exit(0);
               }
               if(sline.size() > 100)
            	   printf("The line will not be included because the sizde is grether than 100 characters \n");
               else
                vSubscriber.push_back(sline);
               index++;
           }
        }
        else
        {
            printf("Error opening file: %s \n", fileNameInput);
            exit(1);
        }
        file.close();

        if(!index )
        {
            printf("Invalid format check the file content or file does not exists \n");
            exit(0);
        }
        if(index > 10000)
        {
        	printf("The file can not be great than 10000 lines \n");
        	exit(0);
        }
        //That means I have one conection per Subscriber
        if(index <= numberThread)
        {
            for(int i = 0; i < index; i++)
            {
                m_instance[i] = new ProducerConsumer(vSubscriber[i].c_str(), order, fileNameOutput);
            }

            for(int i = 0; i < index; i++)
            {
                rc = pthread_create(&threads[i], &attr, RunThread, (void *)m_instance[i]);
                if (rc)
                {
                    cout << "Error:unable to create thread," << rc << endl;
                    exit(-1);
                }
            }
            init = pthread_attr_destroy(&attr);
            if(init != 0)
            {
                printf("Error destroing attributes of thread");
                exit(1);
            }

            for(int i = 0; i < index; i++)
            {
                rc = pthread_join(threads[i], nullptr);
            }
        }
        else //That meaning that are a list of string  per thread
        {
            divition = index / numberThread;
            module = index % numberThread;
            //Run quantity of threads
            for(int i = 0; i < numberThread; i++)
            {
                m_instance[i] = new ProducerConsumer(order, fileNameOutput);
                for(int j = 0; j < divition; j++)
                {
                    m_instance[i]->setVectorSubscriber(vSubscriber[j]);
                }
                vSubscriber.erase(vSubscriber.begin(), vSubscriber.begin()+ divition);
                //That meaning what we have e rest
                if((i==numberThread -1) && (module!=0))
                {
                    for(int l = 0; l < module; l++)
                    {
                        m_instance[i]->setVectorSubscriber(vSubscriber[l]);
                    }
                }
            }
            //Create the threads geted by parameter
            for(int i = 0; i < numberThread; i++)
            {
                rc = pthread_create(&threads[i], &attr, RunThread, (void *)m_instance[i]);
                if (rc)
                {
                    cout << "Error:unable to create thread," << rc << endl;
                    exit(-1);
                }
            }
            init = pthread_attr_destroy(&attr);
            if(init != 0)
            {
                printf("Error destroing attributes of thread");
                exit(1);
            }

            for(int i = 0; i < numberThread; i++)
            {
                rc = pthread_join(threads[i], nullptr);
            }

        }
        //pthread_exit(NULL);
        if(index <= numberThread)
        {
            for(int i = 0; i < index; i++)
                delete m_instance[i];
        }
        else
        {
            for(int i = 0; i < numberThread; i++)
                delete m_instance[i];
        }
    }
    catch (...)
    {
        printf("Exeption unknow \n");
    }

    return 0;
}
