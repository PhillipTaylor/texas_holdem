

#ifndef LOGGING_H
#define LOGGING_H

void logging_init();
void logging_shutdown();

void logging_critical(char *message);
void logging_warning(char *message);
void logging_debug_high(char *message);
void logging_debug_low(char *message);

#endif
