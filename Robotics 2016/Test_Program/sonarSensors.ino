int getLeftDistance() {
  unsigned int uS = sonarLeft.ping();
  int distance = uS / US_ROUNDTRIP_CM;
  return distance;
}

int getRightDistance() {
  unsigned int uS = sonarRight.ping();
  int distance = uS / US_ROUNDTRIP_CM;
  return distance;
}

int getRearDistance() {
  unsigned int uS = sonarRear.ping();
  int distance = uS / US_ROUNDTRIP_CM;
  return distance;
}

