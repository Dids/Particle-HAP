//
//  HKStringBuffer.hpp
//  HKTester
//
//  Created by Lukas Jezny on 19/04/2019.
//  Copyright © 2019 Lukas Jezny. All rights reserved.
//

#ifndef HKStringBuffer_hpp
#define HKStringBuffer_hpp

#include <stdio.h>
#include <string>

class HKStringBuffer {
private:
    char* buffer = NULL;
    int bufferSize = 0;
    int position = 0;
    
public:
    HKStringBuffer(char *buffer, int bufferSize) {
        this->buffer = buffer;
        this->bufferSize =bufferSize;
    }
    
    void clear() {
        memset(this->buffer,0,bufferSize);
    }
    
    char* getBuffer() {
        return this->buffer;
    }
    
    int append(char* data) {
        int len = (int) strlen(data);
        if (position + len <= bufferSize) {
            memcpy(buffer + position, data, len);
            position += len;
            return position;
        }
        return -1;
    }
    
    int append(const char* data) {
        int len = (int) strlen(data);
        if (position + len <= bufferSize) {
            memcpy(buffer + position, data, len);
            position += len;
            return position;
        }
        return -1;
    }
    
    int append(std::string data) {
        return append(data.c_str());
    }
    
    int size() {
        return position;
    }
};

#endif /* HKStringBuffer_hpp */