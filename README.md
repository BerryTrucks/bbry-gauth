# bbry-gauth
Мой домашний проект для самообучения. Нативное приложение Google Authenticator для Blackberry 10. 
Ничего многообещающего. 

Что работает сейчас (2017-10-09):
- Программа запускается;
- Отображается основное лист (с тестовыми данными);
- Отображается страница настроек;
- Отображается страница сканера QR кода;
  - При положительном сканировании закрывается страница сканера;
- Отображается страница ручного ввода кода аутентификации;

Известные проблемы: 
- Программа продолжает есть ресурсы телефона, даже, если была закрыта (не знаю как это решить);
- Страница ручного ввода не закрывается (так как вызываю динамически, пока не знаю как ее закрывать); 
