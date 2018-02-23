#include <iostream>
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

            if(in == sizeof(char*)
            {
                obj[in] = in;
            }
            return obj;
        }

    private:

};