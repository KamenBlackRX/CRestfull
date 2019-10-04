#pragma once

#include <iostream>
#include <vector>
#include <json.h>

using namespace web::json;

template <class T>
class JsonBuilder
{
    public:

        JsonBuilder();
        virtual ~JsonBuilder();

        inline json::value operator << TransformJson(T *in)
        {
            json::value obj;
            //Get value as char* and sent to first key to json
            if(in == sizeof(char*)
            {
                obj[in] = in;
            }

            //Get value as char** array and sent to first key to json
            if(in == sizeof(char**)
            {
                for (auto& x : in )
                {
                    obj[x] = x;
                }
            }

            // Iterate from String Array and create key with it
            if(in == sizeof(std::vector<std::string>))
            {
                std::vector<in>::iterator it;
                for(it = in.begin(); it != in.end(); it++)
                {
                    obj[*it] = *it;
                }
            }
            
            if(in == sizeof(utility::stringstream_t))
            {
                obj.serialize(in);
            }


            return obj;
        }

    private:

};