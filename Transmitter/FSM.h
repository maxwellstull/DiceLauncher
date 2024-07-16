#ifndef FSM_H
#define FSM_H

class FSM
{
  public:
    /// State to know what mode its in
    enum State 
      {
        POLLING = 0,  /// Polling, waiting for button push
        RECHARGE = 1, /// Recharging, will ignore other button pushes until its done recharging
      };
  private:
    /// How long recharge is
    double recharge_ms;
    /// Timer
    double recharge_timer;
    /// Obv
    State current_state;

  public:
    /// Sets recharge duration
    void set_recharge(double amnt)
    {
      recharge_ms = amnt;
    }
    /// Initializes
    void init()
    {
      recharge_timer = recharge_ms;
      current_state = State::POLLING;
    }
    /// Function that runs every time the loop senses a button press
    bool press()
    {
      bool status = false;
      switch(current_state)
      {
        case(State::POLLING):
        {
          current_state = State::RECHARGE; 
          recharge_timer = recharge_ms;
          status = true;
          break;
        }
        case(State::RECHARGE):
        {
          recharge_timer = recharge_ms;
          break;
        }
        default:
        {
          break;
        }
      }
      return status;
    }
    /// Updates timer and potentially changes state
    void update(double time_ms)
    {
      if(current_state == State::RECHARGE)
      {    
        recharge_timer -= time_ms;
        if(recharge_timer <= 0)
        {
          current_state = State::POLLING;
        }
      }
    }
    /// Obv
    int getState()
    {
      return current_state;
    }
};


#endif