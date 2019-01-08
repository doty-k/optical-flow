#include <iostream>
#include <map>
#include <fstream>


std::map<std::string,std::string> loadConfig(std::string filename)
{
    std::ifstream input(filename); //The input stream
    std::map<std::string,std::string> ans; //A map of key-value pairs in the file
    while(input) //Keep on going as long as the file stream is good
    {
        std::string key; //The key
        std::string value; //The value
        std::getline(input, key, ':'); //Read up to the : delimiter into key
        std::getline(input, value, '\n'); //Read up to the newline into value
        // std::string::size_type pos1 = value.find_first_of("\"");
        // Find the first quote in the value
        // std::string::size_type pos2 = value.find_last_of("\"");
        //Find the last quote in the value
        // if(pos1 != std::string::npos && pos2 != std::string::npos && pos2 > pos1)
        //Check if the found positions are all valid
        // {
        // value = value.substr(pos1+1,pos2-pos1-1);
        //Take a substring of the part between the quotes''
            ans[key] = value; //Store the result in the map
        // }
    }
    input.close(); //Close the file stream
    return ans; //And return the result
}
