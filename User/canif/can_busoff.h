/* CAN Bus Off恢复状态机定义 */
typedef enum {
    CAN_BUS_OFF_STATE_IDLE = 0,         // 空闲状态（正常工作）
    CAN_BUS_OFF_STATE_BUS_OFF,          // Bus Off状态
    CAN_BUS_OFF_STATE_FAST_RECOVERY,    // 快恢复阶段
    CAN_BUS_OFF_STATE_SLOW_RECOVERY     // 慢恢复阶段
} CanBusOffStateType;

/* CAN Bus Off恢复配置 */
typedef struct {
    uint16_t fast_recovery_delay_ms;    // 快恢复等待时间（ms）
    uint16_t slow_recovery_delay_ms;    // 慢恢复等待时间（ms）
    uint8_t  max_fast_retries;          // 快恢复最大尝试次数
    uint8_t  max_slow_retries;          // 慢恢复最大尝试次数
} CanBusOffConfigType;

/* CAN Bus Off恢复上下文 */
typedef struct {
    CanBusOffStateType state;           // 当前状态
    uint8_t fast_retry_count;           // 快恢复尝试次数
    uint8_t slow_retry_count;           // 慢恢复尝试次数
    uint32_t recovery_start_time;       // 恢复开始时间戳（ms）
} CanBusOffContextType;