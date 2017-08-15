/*
 * ServerProtocol.h
 *
 *  Created on: Aug 15, 2017
 *      Author: user
 */

#ifndef SRC_SERVERPROTOCOL_H_
#define SRC_SERVERPROTOCOL_H_

#define SHOW_USERS 1
#define SHOW_HS 2
#define START_GAME_WITH_USER 3
#define START_GAME_WITH_RANDOM 4
#define SET_USER_AVAILABLE 5
#define SET_USER_UNAVAILABLE 6
#define DISCONNECT 7





/**
 * PROTOCOL:
 * 1 show users
 * 2 show hs
 * 3 <username> start game with 'username'
 * 4 start game with random user
 * 5 set user as available for game
 * 6 set user as unavailable for game
 * 7 disconnect user
 */
#endif /* SRC_SERVERPROTOCOL_H_ */
