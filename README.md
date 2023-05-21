1. la tarea se ejecuta normalmente yo suelo hacerlo con un "gcc main.c heap.c hashmap.c -o main" y luego un "./main" 

2. Todo parece funcionar correctamente excepto por un error que ocurre en ciertos casos que no he logrado solucionar y me gustaria saber su solucion, hice todo lo que tenia a mi alcance para solucionarlo y ocurre aveces como en el siguiente ejemplo:

Se inicia una tarea con nombre "tarea" se le asigna prioridad 1 sin ninguna tarea precedente, luego creamos una nueva tarea, llamada "tarea2" le asignamos prioridad 3 y una tarea de precededncia que sera "tarea". Luego
le damos a mostrar tareas las muestra pero queda en un loop infinito, anteriormente luego de mostrarlas daba un error de segmentacion. Esto ocurre en la funcion "tareaRealizada", entre que de un loop infinito y un error de 
segmentacion es lo mismo yaque la raiz del error viene a que no puedo acceder a "aux->estado" al principio daba un error de segmentacion por que ingresaba automaticamente luego da un loop por que pregunto si "aux" es null
o si "x" es null y la tarea vuelve a entrar al heap infinitamente cuando deberia dejar de hacerelo, depure con gdb me arrojo donde estaba el error de segmentracion, revise que se estuviera haciendo una buena copia del mapa,
, revise si se estaba copiando corectamente el heap, le pregunte a chat gpt que pasaba y por que no podia acceder al estado de aux, etc.. . sin embargo ese error esta solo en esa funcion y sucede aveces me gustaria saber el por que 

