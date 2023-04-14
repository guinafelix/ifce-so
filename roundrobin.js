class Process {
  constructor(id, burstTime) {
    this.id = id;
    this.burstTime = burstTime;
    this.tempoEspera = 0;
    this.tempoRetorno = 0;
    this.tempoRestante = burstTime;
  }
}

function roundRobin(processes, quantum) {
  const processList = [];
  for (let i = 0; i < processes.length; i++) { 
    processList.push(new Process(processes[i].id, processes[i].burstTime));
  }
  let time = 0;
  let fila = [...processList];
  let filaEncerrados = [];

  while (fila.length > 0) {
    const process = fila.shift();
    time += 1;
    if (process.tempoRestante <= quantum) {
      time += process.tempoRestante;
      process.tempoRestante = 0;
      process.tempoEspera = time - process.burstTime
      process.tempoRetorno = time;
      filaEncerrados.push(process);
    } else {
      time += quantum;
      process.tempoRestante -= quantum;
      fila.push(process);
    }
  }

  const mediaEspera =
    filaEncerrados.reduce((soma, process) => soma + process.tempoEspera, 0) /
    filaEncerrados.length;
  
  const mediaRetorno =
    filaEncerrados.reduce((soma, process) => soma + process.tempoRetorno, 0) /
    filaEncerrados.length;

  const vazao = filaEncerrados.length / time;
  
  //visualizar estado final dos processos
  // filaEncerrados.map((process) => { 
  //    console.log(`Processo ${process.id + 1}
  //    - Burst Time: ${process.burstTime}s
  //    - Tempo de espera: ${process.tempoEspera}s
  //    - Tempo de retorno: ${process.tempoRetorno}s
  //    - Tempo restante: ${process.tempoRestante}s
  //    `);
  // })

  return {
    time,
    mediaEspera,
    mediaRetorno,
    vazao,
  };
}

const numProcessos = 5;
const processos = Array.from(
  { length: numProcessos },
  (_, i) => new Process(i, Math.floor(Math.random() * 5) + 1)
);

for (let quantum = 1; quantum <= 5; quantum++) {
  console.log(`-------------------Quantum ${quantum}---------------`);
  processos.map((process) => { 
    console.log(`Processo ${process.id + 1}
     - Burst Time: ${process.burstTime}s
     - Tempo de espera: ${process.tempoEspera}s
     - Tempo de retorno: ${process.tempoRetorno}s
     - Tempo restante: ${process.tempoRestante}s
     `)
  })
  const valores = roundRobin(processos, quantum);
  console.log(`-------------------Valores---------------`);
  console.log(`Tempo de execução: ${valores.time}s`);
  console.log(`Tempo médio de espera: ${valores.mediaEspera.toFixed(2)}s`);
  console.log(`Tempo médio de retorno: ${valores.mediaRetorno.toFixed(2)}s`);
  console.log(`Vazão: ${valores.vazao.toFixed(4)} processos/s`);
  console.log(`-------------------Fim---------------`);
}
