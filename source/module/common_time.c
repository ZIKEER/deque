#include "common_crc.h"



/* 计量任务需要储存的对象列表 */
static CONST_DEF sParamItem TaskMeasParamItem0109[] = 
{
    /* 任务层需要保存的参数 */
    {(uint16_t)E_D_MEAS_ACCOUNT_TYPE,     sizeof(TaskMeasObj.account_type),  (void*)&TaskMeasObj.account_type,  (const void*)&AccountTypeDefault},
    {(uint16_t)E_D_MEAS_MARGIN_CHARGE_KEY, sizeof(TaskMeasObj.charge_serial_num), (void*)&TaskMeasObj.charge_serial_num, (const void*)0u},
    {(uint16_t)E_D_MEAS_SERVER_PRICE,          sizeof(TaskMeasObj.server_value.price),          (void*)&TaskMeasObj.server_value.price,          (const void*)0u},
    {(uint16_t)E_D_MEAS_SERVER_REMAIN,         sizeof(TaskMeasObj.server_value.remain),         (void*)&TaskMeasObj.server_value.remain,         (const void*)0u},
    {(uint16_t)E_D_MEAS_SERVER_REMAIN_STA,     sizeof(TaskMeasObj.server_value.remain_sta),     (void*)&TaskMeasObj.server_value.remain_sta,     (const void*)0u},
    {(uint16_t)E_D_MEAS_SERVER_VALID_STA,      sizeof(TaskMeasObj.server_value.valid_sta.all),  (void*)&TaskMeasObj.server_value.valid_sta.all,  (const void*)&ServerValidSta},
    {(uint16_t)E_D_MEAS_CHARGE_ERR_STA, sizeof(TaskMeasObj.charge_err_sta), (void*)&TaskMeasObj.charge_err_sta, (const void*)&TaskMeasChargeErrDefault},
    {(uint16_t)E_D_MEAS_MARGIN_CHARGE_TIMESTAMP, sizeof(TaskMeasObj.charge_timestamp), (void*)&TaskMeasObj.charge_timestamp, (const void*)0u},
    {(uint16_t)E_D_MEAS_INC_SOURCE_BUFFER,       sizeof(TaskMeasObj.inc_buffer), (void*)&TaskMeasObj.inc_buffer, (const void*)0u},
    {(uint16_t)E_D_MEAS_PULSE_MODE,               sizeof(PulseManage.param.mode),                   (void*)&PulseManage.param.mode,                   (const void*)&PulseModeDefault},
    {(uint16_t)E_D_MEAS_PULSE_COEFFICIENT,        sizeof(PulseManage.param.coefficient),            (void*)&PulseManage.param.coefficient,            (const void*)&PulseCoefficientDefault},
    {(uint16_t)E_D_MEAS_PULSE_MODULE,             sizeof(PulseManage.param.module),                 (void*)&PulseManage.param.module,                 (const void*)&PulseModuleDefault},
    {(uint16_t)E_D_MEAS_PULSE_FILTER_LIMIT,       sizeof(PulseManage.param.filter_limit),           (void*)&PulseManage.param.filter_limit,           (const void*)&PulseFilterLimitDefault},
    {(uint16_t)E_D_MEAS_PULSE_LOW_POWER_CHECK_EN, sizeof(PulseManage.param.low_power_check_enable), (void*)&PulseManage.param.low_power_check_enable, (const void*)&PulseLpCheckEnDefault},
    {(uint16_t)E_D_MEAS_PULSE_LINE_BROKEN_CHECK_EN, sizeof(PulseManage.param.line_broken_check_enable), (void*)&PulseManage.param.line_broken_check_enable, (const void*)&PulseLineBrokenCheckEnDefault},
    {(uint16_t)E_D_MEAS_PULSE_BUFFER, sizeof(PulseManage.step_buffer), (void*)&PulseManage.step_buffer, (const void*)0u},
    {(uint16_t)E_D_MEAS_FM_INFO,      sizeof(FlowMeterObj.info),          (void*)&FlowMeterObj.info,          (const void*)&FlowMeterInfoDefault},
    {(uint16_t)E_D_MEAS_FM_VALUE_STA, sizeof(FlowMeterObj.value_sta.all), (void*)&FlowMeterObj.value_sta.all, (const void*)&FlowMeterValueStaDefault},
    {(uint16_t)E_D_MEAS_COUNT_MODE,         sizeof(CountObj.param.mode),              (void*)&CountObj.param.mode,              (const void*)&CountModeDefault},/* （设置数据ID时保存） */
    {(uint16_t)E_D_MEAS_COUNT_SYNC_ENABLE,  sizeof(CountObj.param.sync_enable),       (void*)&CountObj.param.sync_enable,       (const void*)&CountSyncEnableDefault},
    {(uint16_t)E_D_MEAS_COUNT_SYNC_LIMIT,   sizeof(CountObj.param.sync_limit),        (void*)&CountObj.param.sync_limit,        (const void*)&CountSyncLimitDefault},
    {(uint16_t)E_D_MEAS_COUNT_ALARM_LIMIT,  sizeof(CountObj.param.alarm_limit),       (void*)&CountObj.param.alarm_limit,       (const void*)&CountAlarmLimitDefault},
    {(uint16_t)E_D_MEAS_COUNT_CONTROLLER_ACCUMULATION, sizeof(CountObj.controller_value), (void*)&CountObj.controller_value, (const void*)0u},
    {(uint16_t)E_D_MEAS_COUNT_AMEND,                   sizeof(CountObj.amend),            (void*)&CountObj.amend,            (const void*)&CountAmendDefault},
    {(uint16_t)E_D_MEAS_COUNT_INTERVAL_CNT,            sizeof(CountObj.collect_interval_cnt), (void*)&CountObj.collect_interval_cnt, (const void*)0u},
    {(uint16_t)E_D_MEAS_COUNT_DAILY_AMOUNT, sizeof(CountObj.daily_amount), (void*)&CountObj.daily_amount, (const void*)0u},
    {(uint16_t)E_D_MEAS_PRICE_LIST,                sizeof(PriceListDefault),     		  	(void*)&PricePlan.para.price_list[0],  (const void*)&PriceListDefault},/* （数据ID设置时保存） */
    {(uint16_t)E_D_MEAS_PRICE_CUR_LIST,            sizeof(PriceCurIndexDefault),        	(void*)&PricePlan.para.current,        (const void*)&PriceCurIndexDefault},/* （价格表变化时保存） */
    {(uint16_t)E_D_MEAS_PRICE_PERIOD_ACCUMULATION, sizeof(PriceCurCycUsedGasDefault),   	(void*)&PricePlan.para.cyc_used_gas,   (const void*)&PriceCurCycUsedGasDefault},/* （每次结算保存） */
    {(uint16_t)E_D_MEAS_PRICE_PERIOD_START_TIME,   sizeof(PriceCycStartTimeDefault), 		(void*)&PricePlan.para.cyc_start_time, (const void*)&PriceCycStartTimeDefault},/*  */
    {(uint16_t)E_D_MEAS_PRICE_CUR_INDEX,           sizeof(PriceCurPhaseDefault),          	(void*)&PricePlan.para.phase,          (const void*)&PriceCurPhaseDefault},/* （价格跨阶梯时保存） */
    {(uint16_t)E_D_MEAS_PRICE_REMAIN_GAS,          sizeof(PriceRemainGasDefault),     		(void*)&PricePlan.para.remain_gas,     (const void*)&PriceRemainGasDefault},/* （每次计算时保存） */
    {(uint16_t)E_D_MEAS_PRICE_ERR_STA,             sizeof(PricePriceErrFlagDefault), 		(void*)&PricePlan.para.price_err_flag, (const void*)&PricePriceErrFlagDefault},/* （价格错误时保存） */
    {(uint16_t)E_D_MEAS_PRICE_IS_FIRST_SET,        sizeof(PricePlan.para.is_first_set),     (void*)&PricePlan.para.is_first_set,   (const void*)0u},
    {(uint16_t)E_D_MEAS_PRICE_SET_ERR_STA,         sizeof(PricePlan.set_err_sta), (void*)&PricePlan.set_err_sta, (const void*)&PriceSetErrDefault},
    {(uint16_t)E_D_MEAS_MARGIN_REMAIN_VALUE,              sizeof(MarginObj.value.remain),        (void*)&MarginObj.value.remain,        (const void*)&MarginRemainDefault},/* （充值或者扣减时保存） */
    {(uint16_t)E_D_MEAS_RESERVE_FAC,  sizeof(MarginFacResObj.value.remain),  (void*)&MarginFacResObj.value.remain,  (const void*)&MarginFacRemainDefault},/* （同上） */
    {(uint16_t)E_D_MEAS_RESERVE_USER, sizeof(MarginUserResObj.value.remain), (void*)&MarginUserResObj.value.remain, (const void*)&MarginUserRemainDefault},/* （同上） */
    {(uint16_t)E_D_MEAS_MARGIN_LIMIT_LEVEL_1,    sizeof(MarginObj.param.level_1.value),   (void*)&MarginObj.param.level_1.value,   (const void*)&MarginLevel1Default},/* （设置数据ID时保存） */
    {(uint16_t)E_D_MEAS_MARGIN_LIMIT_LEVEL_2,    sizeof(MarginObj.param.level_2.value),   (void*)&MarginObj.param.level_2.value,   (const void*)&MarginLevel2Default},/* （同上） */
    {(uint16_t)E_D_MEAS_MARGIN_LIMIT_OVER_DRAFT, sizeof(MarginObj.param.overdraft.value), (void*)&MarginObj.param.overdraft.value, (const void*)&MarginOverDraftDefault},/* （同上） */
    {(uint16_t)E_D_MEAS_MARGIN_EN_LEVEL_1,    sizeof(MarginObj.param.level_1.enable),   (void*)&MarginObj.param.level_1.enable,   (const void*)&DisableDefault},/* （同上） */
    {(uint16_t)E_D_MEAS_MARGIN_EN_LEVEL_2,    sizeof(MarginObj.param.level_2.enable),   (void*)&MarginObj.param.level_2.enable,   (const void*)&DisableDefault},/* （同上） */
    {(uint16_t)E_D_MEAS_MARGIN_EN_OVER_DRAFT, sizeof(MarginObj.param.overdraft.enable), (void*)&MarginObj.param.overdraft.enable, (const void*)&DisableDefault},/* （同上） */
    {(uint16_t)E_D_MEAS_MARGIN_TOTLE_CHARGE_VALUE,  sizeof(MarginObj.value.totle_charge_value), (void*)&MarginObj.value.totle_charge_value, (const void*)0u},/* （充值时保存） */
    {(uint16_t)E_D_MEAS_MARGIN_CHARGE_VALUE, sizeof(MarginObj.value.charge_value),       (void*)&MarginObj.value.charge_value,       (const void*)0u},/* （充值时保存） */
    {(uint16_t)E_D_MEAS_MARGIN_CHARGE_CNT,   sizeof(MarginObj.value.charge_cnt),         (void*)&MarginObj.value.charge_cnt,         (const void*)0u},/* （充值时保存） */
    {(uint16_t)E_D_MEAS_MARGIN_STATE,              sizeof(MarginObj.state),        (void*)&MarginObj.state,        (const void*)0u},
    {(uint16_t)E_D_MEAS_MARGIN_STATE_RESERVE_FAC,  sizeof(MarginFacResObj.state),  (void*)&MarginFacResObj.state,  (const void*)0u},
    {(uint16_t)E_D_MEAS_MARGIN_STATE_RESERVE_USER, sizeof(MarginUserResObj.state), (void*)&MarginUserResObj.state, (const void*)0u},
    {(uint16_t)E_D_MEAS_NO_USE_LVL1,  sizeof(NoUse.para.day1),    (void*)&NoUse.para.day1,    (const void*)&NoUseDayLevel1Default},/* （设置数据ID时保存） */
    {(uint16_t)E_D_MEAS_NO_USE_LVL2,  sizeof(NoUse.para.day2),    (void*)&NoUse.para.day2,    (const void*)&NoUseDayLevel2Default},/* （设置数据ID时保存） */
    {(uint16_t)E_D_MEAS_NO_USE_ENABLE,   sizeof(NoUse.para.en_flag), (void*)&NoUse.para.en_flag, (const void*)&DisableDefault},/* （设置数据ID时保存） */
    {(uint16_t)E_D_MEAS_NO_USE_DAYS,     sizeof(NoUse.run.time_cnt), (void*)&NoUse.run.time_cnt, (const void*)0u},
    {(uint16_t)E_D_MEAS_NO_USE_STATE, sizeof(NoUse.run.flag.all_bit), (void*)&NoUse.run.flag.all_bit, (const void*)0u},        
};