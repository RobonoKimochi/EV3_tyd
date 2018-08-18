#include <kernel.h>
#include "common/module_common.h"
#include "target_config.h"
#include "platform_interface_layer.h"
#include "csl.h"
#include "chip_timer.h"
#include "syssvc/syslog.h"
#include "syssvc/banner.h"
#include "target_serial.h"
#include "target_serial_dbsio.h"
#include "syssvc/serial.h"
#include "syssvc/logtask.h"
#include "api.cfg.h"
#include "app.h"

ID _module_id_APP_INIT_TASK __attribute__((section (".module.text")));
static STK_T _module_ustack_APP_INIT_TASK[COUNT_STK_T(STACK_SIZE)];

ID _module_id_MAIN_TASK __attribute__((section (".module.text")));
static STK_T _module_ustack_MAIN_TASK[COUNT_STK_T(STACK_SIZE)];

ID _module_id_TRACER_TASK __attribute__((section (".module.text")));
static STK_T _module_ustack_TRACER_TASK[COUNT_STK_T(STACK_SIZE)];

ID _module_id_ODMETRY_TASK __attribute__((section (".module.text")));
static STK_T _module_ustack_ODMETRY_TASK[COUNT_STK_T(STACK_SIZE)];

ID _module_id_TAILMOTOR_TASK __attribute__((section (".module.text")));
static STK_T _module_ustack_TAILMOTOR_TASK[COUNT_STK_T(STACK_SIZE)];

ID _module_id_LOGGER_TASK __attribute__((section (".module.text")));
static STK_T _module_ustack_LOGGER_TASK[COUNT_STK_T(STACK_SIZE)];

ID _module_id_REMOTE_TASK __attribute__((section (".module.text")));
static STK_T _module_ustack_REMOTE_TASK[COUNT_STK_T(STACK_SIZE)];

ID _module_id_UI_TASK __attribute__((section (".module.text")));
static STK_T _module_ustack_UI_TASK[COUNT_STK_T(STACK_SIZE)];

static const T_CTSK _module_ctsk_tab[8] = {
	{ TA_ACT, 0, _app_init_task, TPRI_APP_INIT_TASK, ROUND_STK_T(STACK_SIZE), _module_ustack_APP_INIT_TASK, DEFAULT_SSTKSZ, NULL },
	{ TA_ACT, 0, main_task, MAIN_PRIORITY, ROUND_STK_T(STACK_SIZE), _module_ustack_MAIN_TASK, DEFAULT_SSTKSZ, NULL },
	{ TA_NULL, 0, tracer_task, TRACER_PRIORITY, ROUND_STK_T(STACK_SIZE), _module_ustack_TRACER_TASK, DEFAULT_SSTKSZ, NULL },
	{ TA_NULL, 0, odmetry_task, ODMETRY_PRIORITY, ROUND_STK_T(STACK_SIZE), _module_ustack_ODMETRY_TASK, DEFAULT_SSTKSZ, NULL },
	{ TA_NULL, 0, tailmotor_task, TAILMOTOR_PRIORITY, ROUND_STK_T(STACK_SIZE), _module_ustack_TAILMOTOR_TASK, DEFAULT_SSTKSZ, NULL },
	{ TA_NULL, 0, logger_task, LOGGER_PRIORITY, ROUND_STK_T(STACK_SIZE), _module_ustack_LOGGER_TASK, DEFAULT_SSTKSZ, NULL },
	{ TA_NULL, 0, remote_task, REMOTE_PRIORITY, ROUND_STK_T(STACK_SIZE), _module_ustack_REMOTE_TASK, DEFAULT_SSTKSZ, NULL },
	{ TA_NULL, 0, ui_task, UI_PRIORITY, ROUND_STK_T(STACK_SIZE), _module_ustack_UI_TASK, DEFAULT_SSTKSZ, NULL },
};

const SIZE _module_cfg_entry_num = 8;

const MOD_CFG_ENTRY _module_cfg_tab[8] = {
	{ TSFN_CRE_TSK, &_module_ctsk_tab[0], &_module_id_APP_INIT_TASK },
	{ TSFN_CRE_TSK, &_module_ctsk_tab[1], &_module_id_MAIN_TASK },
	{ TSFN_CRE_TSK, &_module_ctsk_tab[2], &_module_id_TRACER_TASK },
	{ TSFN_CRE_TSK, &_module_ctsk_tab[3], &_module_id_ODMETRY_TASK },
	{ TSFN_CRE_TSK, &_module_ctsk_tab[4], &_module_id_TAILMOTOR_TASK },
	{ TSFN_CRE_TSK, &_module_ctsk_tab[5], &_module_id_LOGGER_TASK },
	{ TSFN_CRE_TSK, &_module_ctsk_tab[6], &_module_id_REMOTE_TASK },
	{ TSFN_CRE_TSK, &_module_ctsk_tab[7], &_module_id_UI_TASK },
};

const uint32_t _module_pil_version = PIL_VERSION;
