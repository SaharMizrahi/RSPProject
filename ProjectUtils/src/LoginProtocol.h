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

#ifndef SRC_LOGINPROTOCOL_H_
#define SRC_LOGINPROTOCOL_H_
#define SERVER_PORT 5000
#define SERVER_IP "127.0.0.1"
#define OPEN_FILE_ERROR -2
#define WRONG_COMMAND_ERROR -1
#define LOGIN_APPROVED 0
#define WRONG_PASSWORD_ERROR 1
#define WRONG_USERNAME_ERROR 2
#define REGISTER_APPROVED 3
#define USER_EXIST_ERROR 4




#endif /* SRC_LOGINPROTOCOL_H_ */
