/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Copyright (c) 2019 PCB Arts GmbH - Kevin Schick
 *
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  _____    ____    ____
 *       \  /       |    \        /\            |
 *   ____/  |       |____/       /  \     __  __|__   ___
 *  |       |       |    \      /____\   /      |    |___
 *  |       \____   |____/     /      \  |      |     ___|
 *
 *         _    __   __   __     __         _    __   __
 *  \  /  /_\  |__| |  | |__|   |__| |__|  /_\  |__  |__
 *   \/  /   \ |    |__| | \    |    |  | /   \  __| |__
 *
 *
 *  PCB Arts GmbH
 *  Kurgartenstrasse 59
 *  90762 Fuerth
 *  GERMANY
 *
 *  www.pcb-arts.com
 *  mail(at)pcb-arts.com
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */


#ifndef UI_CLI_COMMANDS_H_
#define UI_CLI_COMMANDS_H_

#ifdef __cplusplus
	extern "C" {
#endif /* __cplusplus */


//=================================
// included dependencies

//=================================
// declarations

extern void cli_commands_execute(const char* buf);
extern int cli_commands_broadcast(volatile char* buf);


enum broadcaster_t {
	BC_OFF = 0,
	BC_TEMP,
	BC_LID_POS,
	BC_LIFT_POS,
	BC_SEQ_PROGRESS,
	BC_DEBUG
};

//lift command
//>lift [up|down|pos|set|cal|min|max|stop|speed]
#define CMD_LIFT "lift"

//lid command
//>lid [up|down|pos|set|cal|min|max|stop|speed]
#define CMD_LID "lid"

//access command
//combined lid and lift control
//>access [open|close|stop]
#define CMD_ACCESS "access"

//move lid/lift up
//>[lid|lift] up
#define CMD_SUB_UP "up"

//mode lid/lift down
//>[lid|lift] down
#define CMD_SUB_DOWN "down"

//mode lid/lift stop | stop sequence
//>[lid|lift|access|seq] stop
#define CMD_SUB_STOP "stop"

//access open
//raises lid, moves lift to optimal access position
//>[access] open
#define CMD_SUB_OPEN "open"

//access close
//lower lid, raise lift to top
//>[access] close
#define CMD_SUB_CLOSE "close"

//mode lid/lift speed (0-100%)
//>[lid|lift] speed
#define CMD_SUB_SPEED "speed"

//show lid/lift position
//>[lid|lift] pos
#define CMD_SUB_POS "pos"

//set lid/lift position, set sequence parameter
//>[lid|lift] set
//>[seq] set (index, temperature, duration)
#define CMD_SUB_SET "set"

//show lid/lift to
//>[lid|lift] to
#define CMD_SUB_TO "to"

//broadcast lid/lift position
//>[lid|lift] bc pos
#define CMD_SUB_BC_POS "bc pos"

//find lid/lift zero position
//>[lid|lift] cal
#define CMD_SUB_CAL "cal"

//min lid/lift position
//>[lid|lift] min [-2.147.483.648 ... 2.147.483.647]
#define CMD_SUB_MIN "min"

//max lid/lift position
//>[lid|lift] max [-2.147.483.648 ... 2.147.483.647]
#define CMD_SUB_MAX "max"

//sequence command
//>seq [start|stop|continue|progress]
#define CMD_SEQUENCE "seq"

//start command
//>[seq] start
#define CMD_SUB_START "start"

//continue command
//>[seq] continue
#define CMD_SUB_CONTINUE "continue"

//abort command, initiate immediate cool down
//>seq abort
#define CMD_SUB_ABORT "abort"

//print the sequence progress
//>[seq] progress
#define CMD_SUB_PROGRESS "progress"

//broadcast the sequence progress
//>[seq] bc progress
#define CMD_SUB_BC_PROGRESS "bc progress"

//heater command
//>heater [reg|en]
#define CMD_HEATER "heater"

//quick cooler command
//>quick cooler[man|auto]
#define CMD_QUICK_COOLER "quick cooler"

//hydro cooler command
//>hydro cooler[man|auto]
#define CMD_HYDRO_COOLER "hydro cooler"

//set fan speed manually
//>[quick cooler|hydro cooler] man [0..100]
#define CMD_SUB_MAN "man"

//enable automatic fan speed controller
//>[quick cooler|hydro cooler] auto
#define CMD_SUB_AUTO "auto"

//set heater temperature
//>[heater|regulator] temp [0-400]
#define CMD_SUB_TEMP "temp"

//set heater temperature controller hysteresis
//>[heater] hyst [0-100]
#define CMD_SUB_HYSTERESIS "hyst"

//pump enable / disable command
//>pump en [1|0]
#define CMD_PUMP_EN "pump en"

//enable/disable regulation
//>[heater] reg [1|0]
#define CMD_SUB_REG "reg"

//PID proportional factor
//>regulator kp [0 .. (2^24 - 1)]
#define CMD_SUB_KP "kp"

//PID integral factor
//>regulator ki [0 .. (2^24 - 1)]
#define CMD_SUB_KI "ki"

//PID derivative factor
//>regulator kd [0 .. (2^24 - 1)]
#define CMD_SUB_KD "kd"

//enable/disable regulation
//disable regulation before using the enable command
//>[heater|regulator] en [1|0]
#define CMD_SUB_EN "en"

//cycle command
//>cycle [show|start|stop]
#define CMD_CYCLE "cycle"

//preset command
//>preset [1..n]
#define CMD_PRESET "preset"

//regulator command
//>regulator [en]
#define CMD_REGULATOR "regulator"

//broadcast temperature readings
//>bc temps
#define CMD_BC_TEMPS "bc temps"

//stop broadcaster
//>bc off
#define CMD_BC_OFF "bc off"


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* UI_CLI_COMMANDS_H_ */
