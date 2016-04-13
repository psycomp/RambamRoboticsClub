int getLeftDistance() {
  unsigned int uS1 = sonarLeft.ping();
  int distance = uS1 / US_ROUNDTRIP_CM;
  return distance;
}

int getRightDistance() {
  unsigned int uS2 = sonarRight.ping();
  int distance = uS2 / US_ROUNDTRIP_CM;
  return distance;
}

int getRearDistance() {
  unsigned int uS3 = sonarRear.ping();
  int distance = uS3 / US_ROUNDTRIP_CM;
  return distance;
}
