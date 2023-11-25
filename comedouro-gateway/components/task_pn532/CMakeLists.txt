idf_component_register(SRCS "task_pn532.c" 
                       INCLUDE_DIRS "include" "../user_pn532/include" "../user_pwm/include"
                       REQUIRES user_pn532 user_pwm)