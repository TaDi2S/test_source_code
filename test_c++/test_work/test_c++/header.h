#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define PACKET_SIZE 188

#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <errno.h>
#include <stdlib.h>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <thread>

#include "LicIssuerTypeDef.h"

using namespace std;