
void lcd_init(void) {
  lcd.begin();
  lcd.backlight();
}


void Q_log(const char *s) {
  lcd.print(s);
}

void S_log(const char *s, int line, bool lclear) {
  if (lclear) lcd.clear();
  lcd.setCursor(0, line);
  Q_log(s);
}
