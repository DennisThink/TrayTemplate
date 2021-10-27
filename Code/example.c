#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <stdbool.h>
#if defined (_WIN32) || defined (_WIN64)
#define TRAY_WINAPI 1
#elif defined (__linux__) || defined (linux) || defined (__linux)
#define TRAY_APPINDICATOR 1
#elif defined (__APPLE__) || defined (__MACH__)
#define TRAY_APPKIT 1
#endif

#include "tray.h"

#if TRAY_APPINDICATOR
#define TRAY_ICON1 "indicator-messages"
#define TRAY_ICON2 "indicator-messages-new"
#elif TRAY_APPKIT
#define TRAY_ICON1 "icon.png"
#define TRAY_ICON2 "icon.png"
#elif TRAY_WINAPI
#define TRAY_ICON1 "icon.ico"
#define TRAY_ICON2 "icon.ico"
#endif

static struct tray tray;

static void toggle_cb(struct tray_menu *item) {
  printf("toggle cb\n");
  item->checked = !item->checked;
  tray_update(&tray);
}

static void hello_cb(struct tray_menu *item) {
  (void)item;
  printf("hello cb\n");
  if (strcmp(tray.icon, TRAY_ICON1) == 0) {
    tray.icon = TRAY_ICON2;
  } else {
    tray.icon = TRAY_ICON1;
  }
  tray_update(&tray);
}

static bool g_thread_run=true;
static void quit_cb(struct tray_menu *item) {
  (void)item;
  printf("quit cb\n");
  g_thread_run = false;
  tray_exit();
}

static void submenu_cb(struct tray_menu *item) {
  (void)item;
  printf("submenu: clicked on %s\n", item->text);
  tray_update(&tray);
}

// Test tray init
/*static struct tray tray = {
    .icon = TRAY_ICON1,
    .tray_text="TRAY_INFO",
    .menu =
        (struct tray_menu[]){
            {.text = "Hello", .cb = hello_cb},
            {.text = "Checked", .checked = 1, .cb = toggle_cb},
            {.text = "Disabled", .disabled = 1},
            {.text = "-"},
            {.text = "SubMenu",
             .submenu =
                 (struct tray_menu[]){
                     {.text = "FIRST", .checked = 1, .cb = submenu_cb},
                     {.text = "SECOND",
                      .submenu =
                          (struct tray_menu[]){
                              {.text = "THIRD",
                               .submenu =
                                   (struct tray_menu[]){
                                       {.text = "7", .cb = submenu_cb},
                                       {.text = "-"},
                                       {.text = "8", .cb = submenu_cb},
                                       {.text = NULL}}},
                              {.text = "FOUR",
                               .submenu =
                                   (struct tray_menu[]){
                                       {.text = "5", .cb = submenu_cb},
                                       {.text = "6", .cb = submenu_cb},
                                       {.text = NULL}}},
                              {.text = NULL}}},
                     {.text = NULL}}},
            {.text = "-"},
            {.text = "Quit", .cb = quit_cb},
            {.text = NULL}},
};*/

static char s_tray_text[64]={'\0'};
static struct tray tray = {
    .icon = TRAY_ICON1,
    .tray_text=s_tray_text,
    .menu =
        (struct tray_menu[]){
            {.text = "Quit", .cb = quit_cb},
            {.text = NULL}},
};

void* thread_func(void * param){
  while(g_thread_run)
  {
    {
      struct timespec time;
      clock_gettime(CLOCK_REALTIME, &time); //获取相对于1970到现在的秒数
      struct tm nowTime;
      localtime_r(&time.tv_sec, &nowTime);
      char current[64]={0};
      sprintf(current, "%04d-%02d-%02d% 02d:%02d:%02d", nowTime.tm_year + 1900, nowTime.tm_mon, nowTime.tm_mday, 
      nowTime.tm_hour, nowTime.tm_min, nowTime.tm_sec);
      strcpy(s_tray_text,current);
    }
    tray_update(&tray);
    sleep(1);
    printf("%s\n",s_tray_text);
  }
  return param;
}

int main() {
  if (tray_init(&tray) < 0) {
    printf("failed to create tray\n");
    return 1;
  }
  pthread_t tid;
  pthread_create(&tid,NULL,thread_func,NULL);
  while (tray_loop(1) == 0) {
    printf("iteration\n");
  }
  return 0;
}

