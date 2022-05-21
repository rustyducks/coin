#ifndef REPLICAHOLDER_H
#define REPLICAHOLDER_H

#include <Coin/Actuator/ActuatorBase.h>

namespace rd {
class ReplicaHolder : public ActuatorBase<ProcedureInput> {
   public:
    ReplicaHolder(ProcedureCommandSenderInterface& procedureSender);

    virtual void updateState(const ProcedureInput& input) override;

    void release();
    void hold();

    enum eState {
        HOLDING,
        RELEASED,
    };
    eState state() { return state_; }
    bool hasReplica() { return hasReplica_; }

   private:
    ProcedureCommandSenderInterface& procedureSender_;
    eState state_;
    bool hasReplica_;
    ProcedureInput::eStatus status_;
};
}  // namespace rd

#endif /* REPLICAHOLDER_H */
