#include "can_busoff.h"

/* 全局恢复上下文（单CAN控制器示例） */
static CanBusOffContextType busOffContext = {
    .state = CAN_BUS_OFF_STATE_IDLE,
    .fast_retry_count = 0,
    .slow_retry_count = 0
};

/* 配置参数（可根据实际需求调整） */
static const CanBusOffConfigType busOffConfig = {
    .fast_recovery_delay_ms = 100,     // 快恢复等待100ms
    .slow_recovery_delay_ms = 1000,    // 慢恢复等待1000ms
    .max_fast_retries = 3,             // 最多尝试3次快恢复
    .max_slow_retries = 5              // 最多尝试5次慢恢复
};

/* 假设的CAN控制器状态检查函数 */
static bool CanControllerIsBusOff(void) {
    // 实际实现应检查CAN控制器的错误状态寄存器
    // 例如：检查FDCAN1->ESR寄存器的BOFF bit
    // return (FDCAN1->ESR & FDCAN_ESR_BOFF) != 0;
    
    // 为了演示，这里模拟一个Bus Off状态
    // 实际应用中需要替换为硬件寄存器读取
    static uint8_t busOffCounter = 0;
    if (busOffCounter < 5) {
        busOffCounter++;
        return false;
    }
    return true; // 模拟Bus Off状态
}

/* 假设的CAN控制器恢复函数 */
static void CanControllerRecover(void) {
    // 实际实现应发送一个隐性位序列来尝试恢复
    // 例如：通过FDCAN控制器的恢复功能
    
    // 为了演示，这里简单模拟恢复过程
    // 实际应用中需要调用硬件寄存器操作
    // FDCAN1->CCCR &= ~FDCAN_CCCR_BOFF;
    // FDCAN1->CCCR |= FDCAN_CCCR_CCE;
    // FDCAN1->CCCR |= FDCAN_CCCR_INIT;
    
    // 恢复成功（模拟）
    // 实际中应检查恢复是否成功
    // return true;
}

/* 检查CAN控制器状态并处理Bus Off */
void CanBusOffHandler(void) {
    uint32_t current_time = GetSystemTimeMs(); // 获取当前系统时间（ms）
    
    switch (busOffContext.state) {
        case CAN_BUS_OFF_STATE_IDLE:
            // 检查是否进入Bus Off状态
            if (CanControllerIsBusOff()) {
                busOffContext.state = CAN_BUS_OFF_STATE_BUS_OFF;
                busOffContext.fast_retry_count = 0;
                busOffContext.slow_retry_count = 0;
                busOffContext.recovery_start_time = current_time;
                // 可选：记录Bus Off事件
                LogBusOffEvent();
            }
            break;
            
        case CAN_BUS_OFF_STATE_BUS_OFF:
            // 检查是否已等待足够时间
            if (current_time - busOffContext.recovery_start_time >= 
                (busOffContext.fast_retry_count < busOffConfig.max_fast_retries ?
                 busOffConfig.fast_recovery_delay_ms : 
                 busOffConfig.slow_recovery_delay_ms)) {
                
                // 尝试恢复
                CanControllerRecover();
                
                // 检查恢复是否成功
                if (!CanControllerIsBusOff()) {
                    // 恢复成功，重置状态
                    busOffContext.state = CAN_BUS_OFF_STATE_IDLE;
                    LogRecoverySuccess();
                } else {
                    // 恢复失败，更新尝试计数
                    if (busOffContext.fast_retry_count < busOffConfig.max_fast_retries) {
                        busOffContext.fast_retry_count++;
                        busOffContext.state = CAN_BUS_OFF_STATE_FAST_RECOVERY;
                    } else {
                        busOffContext.slow_retry_count++;
                        busOffContext.state = CAN_BUS_OFF_STATE_SLOW_RECOVERY;
                    }
                    busOffContext.recovery_start_time = current_time;
                }
            }
            break;
            
        case CAN_BUS_OFF_STATE_FAST_RECOVERY:
            // 快恢复阶段，等待时间已设置，等待时间到后会进入BUS_OFF状态处理
            // 实际处理在BUS_OFF状态中
            break;
            
        case CAN_BUS_OFF_STATE_SLOW_RECOVERY:
            // 慢恢复阶段，等待时间已设置，等待时间到后会进入BUS_OFF状态处理
            // 实际处理在BUS_OFF状态中
            break;
    }
}

/* 主循环中调用 */
// void CanMainLoop(void) {
//     // 其他CAN处理逻辑...
    
//     // 每10ms调用一次Bus Off检测
//     static uint32_t lastCheckTime = 0;
//     if (GetSystemTimeMs() - lastCheckTime >= 10) {
//         CanBusOffHandler();
//         lastCheckTime = GetSystemTimeMs();
//     }
// }

/* 辅助函数（实际应用中需要实现） */
static void LogBusOffEvent(void) {
    // 实现日志记录，例如通过串口或LED
}

static void LogRecoverySuccess(void) {
    // 实现恢复成功日志
}

/* 获取系统时间（毫秒） */
static uint32_t GetSystemTimeMs(void) {
    // 实际实现应使用系统定时器
    return HAL_GetTick();
}