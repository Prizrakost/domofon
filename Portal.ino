void build() {
  // конструктор страницы
  GP.BUILD_BEGIN();
  GP.THEME(GP_DARK);
  
  GP.SWITCH("Door", doorOpen);
  
  GP.BUILD_END();
}

void action() {
  // опрос действий
  if (portal.click()) {
    // опрос кликов
    Serial.println(portal.clickName());
  }
}

void configurePortal() {
  portal.attachBuild(build);
  portal.attach(action);
  portal.enableAuth("admin", "admin");
}
