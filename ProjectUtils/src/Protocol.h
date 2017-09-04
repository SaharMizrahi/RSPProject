			 /* * PROTOCOL:
			  * -2 can't open file
			  * -1 wrong command
			  * 0 login request approved
			  * 1 login-password mistake
			  * 2 login-username not exist
			  * 3 register request approved
			  * 4 register-username already exist*/
/*
 * Login Protocol.h
 *
 *  Created on: Aug 15, 2017
 *      Author: user
 */
#ifndef SRC_PROTOCOL_H_
#define SRC_PROTOCOL_H_
#define SERVER_PORT 5000
#define SERVER_IP "127.0.0.1"
/**login protocol**/
#define OPEN_FILE_ERROR -2
#define WRONG_COMMAND_ERROR -1
#define LOGIN_APPROVED 0
#define WRONG_PASSWORD_ERROR 1
#define WRONG_USERNAME_ERROR 2
#define REGISTER_APPROVED 3
#define USER_EXIST_ERROR 4
#define USER_ALREADY_LOGIN 5

/**server protocol**/
#define SHOW_USERS 1
#define SHOW_HS 2
#define START_GAME_WITH_USER 3
#define START_GAME_WITH_RANDOM 4
#define SET_USER_AVAILABLE 5
#define SET_USER_UNAVAILABLE 6
#define DISCONNECT 7
#define GAME_END 8
#define TEST_ECHO 9

/**game protocol**/
#define ROCK 1
#define SCISSORS 2
#define PAPER 3
#define BYEBYE 4


#define USER1WON 1
#define USER2WON -1
#define DRAW 0
#define USER1CANCLE 4
#define USER2CANCLE 5
#define BOTHCANCLE 6





#endif /* SRC_PROTOCOL_H_ */
