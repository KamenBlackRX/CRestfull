#include <iostream>

#pragma once
// static Struct with default constructor
template <typename T>
class MemoryMapping
{

    public:

        MemoryMapping()
        {

        }

        ~MemoryMapping()
        {

        }


        /**
         * convert All types for string
         * @param K Object to be converted.
         * @return string  Return string from object.
         */
        template<typename K>
        inline std::string ToString(K k)
        {

            std::stringstream ss;
            ss << k;
            return ss.str();
        }

        /**
         * Return a report with memory resume
         * @param std::string flag  Flag to set type of memory count (Kb, Mb, Gb)
         * @return T  Return a generic type.
         */
        inline T GetMemoryResume(std::string flag)
        {
            // Recive as String
            try
            {

                std::string memory = ToString<size_t>(GetCurrentRSSMemory());

                if(memory.empty())
                {
                    throw;
                }

                if (typeid(T).name() == "Ss")
                {

                    if (flag == "M")
                    {
                        std::cout << "Memory Virtual: " << memory << " Mb." << std::endl;
                        std::cout << "Memory Res: " << memory << " Mb." << std::endl;
                    }
                    else
                    {

                        std::cout << "Memory Virtual: " << memory << " Kb." << std::endl;
                        std::cout << "Memory Res: " << memory << " Kb." << std::endl;
                    }

                    return memory;

                }
                else if (typeid(T).name() == "size_t")
                {
                    std::cout << "Not Imeplemented.";
                    return 0L;
                }

                return "NULL";

            }
            catch(std::exception& ex)
            {
                std::cout << "Execption has raised.\nDetails: " << ex.what() << std::endl;

            }
        }

    protected:
        /**
         * Get Curren Residensial memory with current PID
         * @param int* pid  Current given PID to revice memory mapping
         * @return size_t  Return current Rss value from memory mapping kernel.
         */
        inline size_t GetCurrentRSSMemory()
        {
            FILE *fp = NULL;
            long rss = 0L;

            if ((fp = fopen("/proc/self/statm", "r")) == NULL)
                return (size_t)0L; /* Can't open? */

            if (fscanf(fp, "%*s%ld", &rss) != 1)
            {
                fclose(fp);
                return (size_t)0L; /* Can't read? */
            }

            fclose(fp);
            return (size_t)rss * (size_t)sysconf(_SC_PAGESIZE);
        }
};
