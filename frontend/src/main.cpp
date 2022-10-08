#include "MyApp.h"
#include "../../backend/BlockChain.h"

int main() {
  MyApp app;
  app.Run();

  BlockChain<10> bc;
  bc.push("Renato", "Joaquin", 777, 10020);

  return 0;
}
