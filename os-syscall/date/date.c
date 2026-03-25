#include "types.h"
#include "user.h"
#include "date.h"

int main(int argc, char *argv[]) {



    struct rtcdate result;
    int val = datetime(&result);

    //%Y-%m-%dT%H:%M:%S
    printf("datetime is %04d-%02d-%02dT%02d%02d%02d\n", result.year, result.month, result.day, result.hour, result.minute, result.second);

    // Your code here
    exit();
}
