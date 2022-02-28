class Menu_Files {


  public:

  void MainMenu(int menu){

   switch (menu) {
    case 0:
      menu = 1;
      break;
    case 1:
      lcd.clear();
      lcd.print(">Incubation");
      lcd.setCursor(0, 1);
      lcd.print(" Settings");
      lcd.setCursor(0, 2);
      lcd.print(" Display");
      lcd.setCursor(0, 3);
      lcd.print(" Exit");
      break;
    case 2:
      lcd.clear();
      lcd.print(" Incubation");
      lcd.setCursor(0, 1);
      lcd.print(">Settings");
      lcd.setCursor(0, 2);
      lcd.print(" Display");
      lcd.setCursor(0, 3);
      lcd.print(" Exit");
      break;
    case 3:
      lcd.clear();
      lcd.print(" Incubation");
      lcd.setCursor(0, 1);
      lcd.print(" Settings");
      lcd.setCursor(0, 2);
      lcd.print(">Display");
      lcd.setCursor(0, 3);
      lcd.print(" Exit");
      break;
    case 4:
      lcd.clear();
      lcd.print(" Incubation");
      lcd.setCursor(0, 1);
      lcd.print(" Settings");
      lcd.setCursor(0, 2);
      lcd.print(" Display");
      lcd.setCursor(0, 3);
      lcd.print(">Exit");
      break;
    case 5:
      menu = 4;
      break;
  }
    }

    void SubMainMenuIncubation(int subMenu){

  lcd.clear();
  switch (subMenu) {
    case 1:
      lcd.clear();
      lcd.print(">Chicken");
      lcd.setCursor(0, 1);
      lcd.print(" GuineaFowl");
      lcd.setCursor(0, 2);
      lcd.print(" Back");
      break;

    case 2:
      lcd.clear();
      lcd.print(" Chicken");
      lcd.setCursor(0, 1);
      lcd.print(">GuineaFowl");
      lcd.setCursor(0, 2);
      lcd.print(" Back");
      break;

    case 3:
      lcd.clear();
      lcd.print(" Chicken");
      lcd.setCursor(0, 1);
      lcd.print(" GuineaFowl");
      lcd.setCursor(0, 2);
      lcd.print(">Back");
      break;
  }

      }


      void SubMainMenuSettings(int subMenu){

 lcd.clear();
  switch (subMenu) {
    case 1:
      lcd.clear();
      lcd.print(">Language");
      lcd.setCursor(0, 1);
      lcd.print(" Back");
      break;

    case 2:
      lcd.clear();
      lcd.print(" Language");
      lcd.setCursor(0, 1);
      lcd.print(">Back");
      break;

  }
      
      }


      
      void languageSettings(int subMenu){

 lcd.clear();
  switch (subMenu) {
    case 1:
      lcd.clear();
      lcd.print(">English");
      lcd.setCursor(0, 1);
      lcd.print(" Swahili");
      lcd.setCursor(0, 2);
      lcd.print(" Back");
      break;

    case 2:
       lcd.clear();
      lcd.print(" English");
      lcd.setCursor(0, 1);
      lcd.print(">Swahili");
      lcd.setCursor(0, 2);
      lcd.print(" Back");
      break;

      case 3:
       lcd.clear();
      lcd.print(" English");
      lcd.setCursor(0, 1);
      lcd.print(" Swahili");
      lcd.setCursor(0, 2);
      lcd.print(">Back");
      break;

  }
      
      }


      void Start_Back(bool processInitiated, int startStatus){

  lcd.clear();
  //condition to be changed later for starting and stoping the incubation process
  if(!processInitiated){
  switch (startStatus) {
    case 1:
      lcd.clear();
      lcd.print(">Start");
      lcd.setCursor(0, 1);
      lcd.print(" Back");
      break;

    case 2:
      lcd.clear();
      lcd.print(" Start");
      lcd.setCursor(0, 1);
      lcd.print(">Back");
      break;
  }}

  else{

    switch (startStatus) {
    case 1:
      lcd.clear();
      lcd.print(">Stop");
      lcd.setCursor(0, 1);
      lcd.print(" Back");
      break;

    case 2:
      lcd.clear();
      lcd.print(" Stop");
      lcd.setCursor(0, 1);
      lcd.print(">Back");
      lcd.setCursor(0, 2);
      break;
  }
    }
        
        }
  };

  Menu_Files MF;
