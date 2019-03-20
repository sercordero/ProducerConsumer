//
// Created by cordese on 13/2/2019.
//

#include "ProducerConsumer.hpp"

ProducerConsumer::ProducerConsumer(const char* line, int id, char * name)
{
    string  strid;
    strid = line;
    order = id;
    sprintf(fileOut,"%s", name);
    setVectorSubscriber(strid);
 }

ProducerConsumer::ProducerConsumer(int id, char * name)
{
	order = id;
	sprintf(fileOut,"%s", name);
}

ProducerConsumer::~ProducerConsumer()
{
}


void ProducerConsumer::setVectorSubscriber(string vSubscriber)
{
	vStrid.push_back(vSubscriber);

}

void ProducerConsumer::orderVector(char ordered)
{
    int middle;
    vector<string> tmp1;
    string tmp(1,ordered);
    vector<string>::iterator iterator1;
    iterator1 = vStrimsi.begin();

    std::size_t tam = vStrimsi.size();
    if(tam > 1)
    {
        middle = tam / 2;
        if(vStrimsi[middle] < tmp)
        {
           for(int i=middle; i<(int)vStrimsi.size(); i++)
		   {
			   if(vStrimsi[i]>tmp)
			   {
				   vStrimsi.insert(iterator1 + i, tmp);
				   return;
			   }
		   }
           if(vStrimsi[vStrimsi.size()-1] <= tmp)
           {
        	   vStrimsi.push_back(tmp);
        	   return;
           }
        }
        else
        {
        	for(int i=0; i<middle; i++)
			{
			   if(vStrimsi[i]>=tmp){
				   vStrimsi.insert(iterator1 + i, tmp);
				   return;

			   }
			}
	    }
    }
    else
    {
    	if(tam == 1)
    	{
    		if(vStrimsi[0] < tmp){
    			vStrimsi.push_back(tmp);
    			return;
    		}
    		else{
    			vStrimsi.insert(iterator1, tmp);
    			return;
    		}
    	}
    	else{
    		vStrimsi.push_back(tmp);
    		return;
    	}

    }

}


void ProducerConsumer::run()
{
     try {
        string tmp;
        std::mutex mtx;

            for(int i = 0; i < (int)vStrid.size(); i++)
            {
                tmp = vStrid[i];
                 vStrimsi.clear();
                for(int j=0; j<(int)tmp.size(); j++)
                {
                	if(tmp[j] == ' ')
					{
                		mtx.lock();
						printf("slepp \n");
                		sleep(1);
                		mtx.unlock();
					}
                	else
                		orderVector(tmp[j]);
                }


            }
            for(int g=0; g<(int)vStrimsi.size(); g++)
                 printf("%s\n", vStrimsi[g].c_str());
            printVector();

    }
    catch (...)
    {
        throw;
    }
}

void ProducerConsumer::printVector()
{
	std::mutex mtx;

	mtx.lock();
	outfile.open(fileOut, std::ofstream::out| ios::app );
	if(outfile.is_open())
	{
		for(int g=0; g<(int)vStrimsi.size(); g++)
		{
			outfile << vStrimsi[g].c_str();
			outfile << ",";
		}
		outfile << "\n";
		outfile.close();
	}
	mtx.unlock();


}
