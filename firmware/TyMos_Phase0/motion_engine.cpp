#include "motion_engine.h"
#include "core_settings_manager.h"
#include "utils_logger.h"

MotionEngine::MotionEngine(MotionServo *servo, MotionCollision *collision) {
  _servo = servo;
  _collision = collision;
}

void MotionEngine::tick() {
  // Forward tick to servo manager for idle checks
  _servo->checkIdle();
}

void MotionEngine::resetSequence() {
  // Delay between each segment movement (as per reference sketch)
  const int PAUSA_MOVIMENTO = 500;

  Logger.info("Starting Reset Sequence...");
  Logger.info("------------------------------------------");
  Logger.info("FASE 1: AZZERAMENTO - Tutti a RIPOSO");
  Logger.info("------------------------------------------");

  // Phase 1 (REST): DO -> UO -> SEP -> DM -> UM. Segments 1 to 7 one by one.

  // DO: segments 1->7 to REST
  Logger.info("DO - Portando segmenti 1->7 a RIPOSO...");
  _setDigitStateSequential(DIGIT_DO, false, false, PAUSA_MOVIMENTO);

  // UO: segments 1->7 to REST
  Logger.info("UO - Portando segmenti 1->7 a RIPOSO...");
  _setDigitStateSequential(DIGIT_UO, false, false, PAUSA_MOVIMENTO);

  // SEP to REST
  Logger.info("SEP - Portando a RIPOSO...");
  _setSeparatorState(false);
  delay(PAUSA_MOVIMENTO);

  // DM: segments 1->7 to REST
  Logger.info("DM - Portando segmenti 1->7 a RIPOSO...");
  _setDigitStateSequential(DIGIT_DM, false, false, PAUSA_MOVIMENTO);

  // UM: segments 1->7 to REST
  Logger.info("UM - Portando segmenti 1->7 a RIPOSO...");
  _setDigitStateSequential(DIGIT_UM, false, false, PAUSA_MOVIMENTO);

  Logger.info("*** AZZERAMENTO COMPLETATO ***");
  Logger.info("Pausa di 2 secondi...");
  delay(2000); // Pause between phases

  Logger.info("------------------------------------------");
  Logger.info("FASE 2: ATTIVAZIONE - Tutti ad ATTIVO");
  Logger.info("------------------------------------------");

  // Phase 2 (ACTIVE): UM -> DM -> SEP -> UO -> DO. Segments 7 to 1 one by one.

  // UM: segments 7->1 to ACTIVE
  Logger.info("UM - Portando segmenti 7->1 ad ATTIVO...");
  _setDigitStateSequential(DIGIT_UM, true, true, PAUSA_MOVIMENTO);

  // DM: segments 7->1 to ACTIVE
  Logger.info("DM - Portando segmenti 7->1 ad ATTIVO...");
  _setDigitStateSequential(DIGIT_DM, true, true, PAUSA_MOVIMENTO);

  // SEP to ACTIVE
  Logger.info("SEP - Portando ad ATTIVO...");
  _setSeparatorState(true);
  delay(PAUSA_MOVIMENTO);

  // UO: segments 7->1 to ACTIVE
  Logger.info("UO - Portando segmenti 7->1 ad ATTIVO...");
  _setDigitStateSequential(DIGIT_UO, true, true, PAUSA_MOVIMENTO);

  // DO: segments 7->1 to ACTIVE
  Logger.info("DO - Portando segmenti 7->1 ad ATTIVO...");
  _setDigitStateSequential(DIGIT_DO, true, true, PAUSA_MOVIMENTO);

  Logger.info("*** TEST 1 COMPLETATO - Display: 88:88 ***");

  delay(2000); // Pause before second test

  // ========================================
  // TEST 2: Number sequence 0-9 with collision logic
  // ========================================
  Logger.info("------------------------------------------");
  Logger.info("TEST 2: Sequenza numeri 0-9");
  Logger.info("------------------------------------------");

  // Current state: all digits show 8 (all segments active)
  // Sequence: DO->0, UO->1, DM->2, UM->3, DO->4, UO->5, DM->6, UM->7, DO->8, UO->9

  const DigitPosition digitOrder[4] = {DIGIT_DO, DIGIT_UO, DIGIT_DM, DIGIT_UM};
  const char *digitNames[4] = {"DO", "UO", "DM", "UM"};

  // Track current displayed number for each digit (all start at 8)
  int currentNum[4] = {8, 8, 8, 8};

  // Show numbers 0-9 sequentially across digits
  for (int num = 0; num <= 9; num++) {
    int digitIdx = num % 4; // Cycle through DO, UO, DM, UM
    DigitPosition digit = digitOrder[digitIdx];

    Logger.info("%s -> %d (da %d)", digitNames[digitIdx], num,
                currentNum[digitIdx]);

    // Use updateDigit which handles collision logic for segment 7
    updateDigit(digit, currentNum[digitIdx], num);
    currentNum[digitIdx] = num;

    delay(500); // Pause between each number change
  }

  Logger.info("*** Sequenza 0-9 completata ***");

  delay(1000);

  // ========================================
  // Return to 88:88 (all segments active)
  // ========================================
  Logger.info("------------------------------------------");
  Logger.info("Ritorno a 88:88");
  Logger.info("------------------------------------------");

  // Final state after 0-9 sequence:
  // DO: 8 (from step 8), UO: 9 (from step 9), DM: 6 (from step 6), UM: 7 (from step 7)
  // Need to return all to 8

  // UO: 9 -> 8
  Logger.info("UO -> 8 (da 9)");
  updateDigit(DIGIT_UO, 9, 8);
  delay(500);

  // DM: 6 -> 8
  Logger.info("DM -> 8 (da 6)");
  updateDigit(DIGIT_DM, 6, 8);
  delay(500);

  // UM: 7 -> 8
  Logger.info("UM -> 8 (da 7)");
  updateDigit(DIGIT_UM, 7, 8);
  delay(500);

  // DO is already at 8

  Logger.info("==========================================");
  Logger.info("   TEST COMPLETATO - Display: 88:88");
  Logger.info("==========================================");
}

void MotionEngine::updateDigit(DigitPosition digit, int fromNum, int toNum) {
  if (fromNum == toNum)
    return;

  // Check for collision logic
  if (_collision->needsCollisionLogic(fromNum, toNum)) {
    // Collision Sequence (Blocking) - handles speed internally
    _collision->executeSequence(digit, fromNum, toNum);
  } else {
    // Normal Update (Non-collision)
    bool segsFrom[7];
    bool segsTo[7];
    MotionSegmentMap::getSegmentsForDigit(fromNum, segsFrom);
    MotionSegmentMap::getSegmentsForDigit(toNum, segsTo);

    SpeedProfile speed = Settings.getSpeed();

    for (int i = 0; i < 7; i++) { // Segments 1-7 (Indices 0-6)
      int seg = i + 1;
      if (segsFrom[i] != segsTo[i]) {
        // State changed
        uint8_t b, c;
        MotionSegmentMap::getChannel(digit, seg, b, c);

        SegmentConfig cfg = MotionSegmentMap::getAngles(seg);
        int startAngle = segsFrom[i] ? cfg.active : cfg.rest;
        int targetAngle = segsTo[i] ? cfg.active : cfg.rest;

        _servo->moveToAngle(b, c, startAngle, targetAngle, speed);
        delay(SERVO_STAGGER_DELAY_MS);
      }
    }
  }
}

void MotionEngine::setSeparator(bool active) {
  _setSeparatorState(active);
}

void MotionEngine::_setDigitState(DigitPosition digit, bool active,
                                  bool reverseOrder) {
  // Iterate segments 1-7 or 7-1
  int start = reverseOrder ? 7 : 1;
  int end = reverseOrder ? 1 : 7;
  int step = reverseOrder ? -1 : 1;

  SpeedProfile speed = Settings.getSpeed();

  for (int seg = start; seg != (end + step); seg += step) {
    uint8_t b, c;
    MotionSegmentMap::getChannel(digit, seg, b, c);

    SegmentConfig cfg = MotionSegmentMap::getAngles(seg);
    int startAngle = active ? cfg.rest : cfg.active;
    int targetAngle = active ? cfg.active : cfg.rest;

    _servo->moveToAngle(b, c, startAngle, targetAngle, speed);
    delay(SERVO_STAGGER_DELAY_MS);
  }
}

void MotionEngine::_setDigitStateSequential(DigitPosition digit, bool active,
                                            bool reverseOrder, int delayMs) {
  // Move segments one by one with specified delay (for reset sequence test)
  // Iterate segments 1-7 or 7-1
  int start = reverseOrder ? 7 : 1;
  int end = reverseOrder ? 1 : 7;
  int step = reverseOrder ? -1 : 1;

  for (int seg = start; seg != (end + step); seg += step) {
    uint8_t b, c;
    MotionSegmentMap::getChannel(digit, seg, b, c);

    SegmentConfig cfg = MotionSegmentMap::getAngles(seg);
    int target = active ? cfg.active : cfg.rest;

    Logger.info("  Segmento %d -> %s (%d)", seg, active ? "ATTIVO" : "RIPOSO",
                target);
    _servo->setAngle(b, c, target);
    delay(delayMs);
  }
}

void MotionEngine::_setSeparatorState(bool active) {
  uint8_t b, c;
  MotionSegmentMap::getChannelSeparator(b, c);

  int target = active ? ANGLE_ACTIVE_STANDARD : ANGLE_REST_STANDARD;
  _servo->setAngle(b, c, target);
}
