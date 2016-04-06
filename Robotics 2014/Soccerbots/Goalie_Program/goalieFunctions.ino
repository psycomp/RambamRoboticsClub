void goalieCharge() {
  DEBUG_PRINT("CHARGE!"); 
  goalieState = CHARGING;
  drive(0, 50, 0);  // CHARGE
  goalieEvent = t.after(1500, goalieRetreat);
}

void goalieRetreat() {
  DEBUG_PRINT("RETREAT!"); 
  halt();
  goalieState = RETREATING;
  drive(0, -50, 0);  // RETREAT
  goalieEvent = t.after(1350, goalieStop);
}

void goalieStop() {
  DEBUG_PRINT("STOP!"); 
  halt();
  turnTo(0);
  goalieState = TRACKING;
  t.stop(goalieEvent); // in case we're doing the goalie-dance
}
