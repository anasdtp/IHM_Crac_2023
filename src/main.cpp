#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "TS_DISCO_F469NI.h"
#include "LCD_DISCO_F469NI.h"
#include <cstring>
#include "threadSD.h"
#include "threadCAN.h"

LCD_DISCO_F469NI lcd;
TS_DISCO_F469NI ts;

DigitalOut led (LED1);

/*
void return_error (int ret_val)
{
    char buf[30];
    if (ret_val) {
        sprintf(buf, "Fail %d", ret_val);
        lcd.DisplayStringAt(0, LINE(8), (uint8_t *)buf, CENTER_MODE);
    } else {
        lcd.DisplayStringAt(0, LINE(8), (uint8_t *)"Done", CENTER_MODE);
    }
}

void errno_error(void* ret_val)
{
    char buf[30];
    if (ret_val == NULL) {
        sprintf(buf, "Fail %d", errno);
        lcd.DisplayStringAt(0, LINE(9), (uint8_t *)buf, CENTER_MODE);
    } else {
        lcd.DisplayStringAt(0, LINE(9), (uint8_t *)"Done", CENTER_MODE);
    }
}
*/

int main ()
{
    ThreadCAN threadCAN(true);
    ThreadSD threadSD;
    threadSD.registerCANControl(threadCAN);

    char buf[100];

    printf("Début du programme\r\n");
    lcd.DisplayStringAt(0, LINE(0), (uint8_t *)"START SD DEMO", CENTER_MODE);
    lcd.DisplayStringAt(0, LINE(2), (uint8_t *)"SD INIT", CENTER_MODE);

    int nb = 0;
    while (1) {
        int flag;
        flag = threadSD.status();
        sprintf(buf, "flag = %04X", flag);
        lcd.DisplayStringAt(0, LINE(4), (uint8_t *)buf, CENTER_MODE);
        if (threadSD.waitReady(1000)) break;
        if (nb++ >= 600) NVIC_SystemReset();
    }

    threadSD.waitReady();
    threadSD.ls();
    lcd.DisplayStringAt(0, LINE(6), (uint8_t *)threadSD.getReply(), CENTER_MODE);
    printf("> Msg LS : %s\r\n", threadSD.getReply());
    /*
        threadSD.cdNum(0);
        lcd.DisplayStringAt(0, LINE(7), (uint8_t *)threadSD.getReply(), CENTER_MODE);
        printf("> Msg CD 0 : %s\r\n", threadSD.getReply());
        threadSD.cdParent();
        lcd.DisplayStringAt(0, LINE(8), (uint8_t *)threadSD.getReply(), CENTER_MODE);
        printf("> Msg CD parent : %s\r\n", threadSD.getReply());
        threadSD.cdName("fichiers");
        lcd.DisplayStringAt(0, LINE(9), (uint8_t *)threadSD.getReply(), CENTER_MODE);
        printf("> Msg CD fichiers : %s\r\n", threadSD.getReply());
        threadSD.rmName("data.txt");
        lcd.DisplayStringAt(0, LINE(10), (uint8_t *)threadSD.getReply(), CENTER_MODE);
        printf("> Msg del zozo : %s\r\n", threadSD.getReply());
        threadSD.cdRoot();
        lcd.DisplayStringAt(0, LINE(11), (uint8_t *)threadSD.getReply(), CENTER_MODE);
        printf("> Msg CD root : %s\r\n", threadSD.getReply());
        threadSD.ls();
        lcd.DisplayStringAt(0, LINE(12), (uint8_t *)threadSD.getReply(), CENTER_MODE);
        printf("> Msg LS : %s\r\n", threadSD.getReply());
    */
    while (1) {
        ThisThread::sleep_for(1000ms);
    }

    /*
        sprintf(buf, "Format\r\n");
        pc.write(buf, strlen(buf));
        error = FATFileSystem::format(&bd);
        return_error(error);
        sprintf(buf, "  Error = %d\r\n", error);
        pc.write(buf, strlen(buf));
    */
//
    /*
        lcd.DisplayStringAt(0, LINE(0), (uint8_t *)"TOUCHSCREEN SD DEMO", CENTER_MODE);
        lcd.SetFont(&Font12);
        lcd.DisplayStringAt(0, LINE(2), (uint8_t *)"Start", CENTER_MODE);
        wait(1);
    */
    /*
        // Format
        error = FATFileSystem::format(&bd);
        return_error(error);

    // Mount
    lcd.DisplayStringAt(0, LINE(3), (uint8_t *)"Mounting the filesystem on \"/fs\". ", CENTER_MODE);
    error = fs.mount(&bd);
    return_error(error);

        lcd.DisplayStringAt(0, LINE(4), (uint8_t *)"Opening a new file, numbers.txt.", CENTER_MODE);
        fd = fopen("/fs/numbers.txt", "w");
        errno_error(fd);

        for (int i = 0; i < 20; i++) {
            sprintf(buf, "Writing decimal numbers to a file (%2d/20)", i+1);
            lcd.DisplayStringAt(0, LINE(5), (uint8_t *)buf, CENTER_MODE);
            fprintf(fd, "%d\r\n", i);
        }

        fclose(fd);
        printf(" done.\r\n");
        lcd.DisplayStringAt(0, LINE(6), (uint8_t *)"File closed", CENTER_MODE);
    */
    /*    fd = fopen("/fs/numbers.txt", "r");
        errno_error(fd);

        printf("Dumping file to screen.\r\n");
        char buff[16] = { 0 };
        while(!feof (fd)) {
            int size = fread(buff, 1, 1, fd);
    //        pc.putc(buff[0]);
    //        fwrite(&buff[0], 1, size, stdout);
        }
        pc.printf("EOF.\r\n");

        pc.printf("Closing file.");
        fclose(fd);
        pc.printf(" done.\r\n");

        pc.printf("Opening root directory.");
        DIR* dir = opendir("/fs/");
        errno_error(fd);

        struct dirent* de;
        pc.printf("Printing all filenames:\r\n");
        while((de = readdir (dir)) != NULL) {
            pc.printf("  %s\r\n", de->d_name);
        }

        pc.printf("Closeing root directory. ");
        error = closedir(dir);
        return_error(error);
        pc.printf("Filesystem Demo complete.\r\n");

        // Blink led with 2 Hz
        while(true) {
            led = !led;
            wait (0.5);
        }*/
}
