//
//  StringUtil.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 3/9/17.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__StringUtil__
#define __noctisgames__StringUtil__

#include <string>

class StringUtil
{
public:
    static std::string encryptDecrypt(std::string input)
    {
        char key[3] = {'N', 'G', 'S'}; // Any chars will work, in an array of any size
        std::string output = input;
        
        for (int i = 0; i < input.size(); i++)
        {
            output[i] = input[i] ^ key[i % (sizeof(key) / sizeof(char))];
        }
        
        return output;
    }
};

#endif /* defined(__noctisgames__StringUtil__) */
