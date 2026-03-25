#include "types.h"
#include "user.h"
#include "date.h"

int main(int argc, char *argv[]) {



    struct rtcdate result;
    int val = datetime(&result);
    if (val==-1) {
        exit();
    }

    //%Y-%m-%dT%H:%M:%S
    printf(1, "datetime is %04d-%02d-%02dT%02d%02d%02d\n", result.year, result.month, result.day, result.hour, result.minute, result.second);

    // Your code here
    exit();
}
