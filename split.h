#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <cstdio>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/time.h>

using namespace std;
int split(const string &txt, vector<string> &strs);