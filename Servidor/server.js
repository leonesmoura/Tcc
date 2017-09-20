var express = require('express'); //Framework para criar o servidor
var request = require('request'); //Biblioteca para fazer requisi��o dos ESPs

var app = express();

// Adicionando M�todos Http.
app.use(function (req, res, next) {
    res.setHeader('Access-Control-Allow-Origin', '*');
    res.setHeader('Access-Control-Allow-Methods', 'GET, POST, OPTIONS, PUT, PATCH, DELETE');
    res.setHeader('Access-Control-Allow-Headers', 'X-Requested-With,content-type');
    res.setHeader('Access-Control-Allow-Credentials', true);
    next();
});

app.use('/', express.static('public')); //Serve a p�gina para os usu�rios. Deve haver uma pasta com o nome "public" no mesmo local do servidor e nela conter a p�gina html

app.get('/vls1', (req, res) =>{ //caso o caminho seja /vls1, mostrar as informa��es adquiridas
	  res.send('Recebido Mudan�a na L�mpada Sala 1');
		console.log(req.query);
});
 
app.get('/vls2', (req, res) =>{ //caso o caminho seja /vls2, mostrar as informa��es adquiridas
	  res.send('Recebido Mudan�a na L�mpada Sala 2');
		console.log(req.query);
});
 
app.get('/vlv', (req, res) =>{ //caso o caminho seja /vlv, mostrar as informa��es adquiridas
	  res.send('Recebido Mudan�a na L�mpada Varanda');
		console.log(req.query);
});
 
app.get('/vlq1', (req, res) =>{ //caso o caminho seja /vlq1, mostrar as informa��es adquiridas
     res.send('Recebido Mudan�a na L�mpada Quarto 1');
	  console.log(req.query);
});
 
app.get('/vlq2', (req, res) =>{ //caso o caminho seja /vlq2, mostrar as informa��es adquiridas
      res.send('Recebido Mudan�a na L�mpada Quarto 2');
      console.log(req.query);
});
 
app.get('/vlc', (req, res) =>{ //caso o caminho seja /vlc, mostrar as informa��es adquiridas
	  res.send('Recebido Mudan�a na L�mpada Cozinha');
		console.log(req.query);
});

app.get('/vlb', (req, res) =>{ //caso o caminho seja /vlb, mostrar as informa��es adquiridas
	  res.send('Recebido Mudan�a na L�mpada banheiro');
		console.log(req.query);
});

app.get('/vlst', (req, res) =>{ //caso o caminho seja /vlst, mostrar as informa��es adquiridas
	  res.send('Recebido Mudan�a na L�mpada Su�te');
		console.log(req.query);
});

app.get('/vlas', (req, res) =>{ //caso o caminho seja /vlas, mostrar as informa��es adquiridas
	  res.send('Recebido Mudan�a na L�mpada �rea de Servi�o');
		console.log(req.query);
});
 
app.get('/vtq1', (req, res) =>{ //caso o caminho seja /vtq1, mostrar as informa��es adquiridas
	  res.send('Recebido Mudan�a na Tomada Quarto 1');
		console.log(req.query);
});

app.get('/vtq2', (req, res) =>{ //caso o caminho seja /vtq2, mostrar as informa��es adquiridas
	  res.send('Recebido Mudan�a na Tomada Quarto 2');
		console.log(req.query);
});

app.get('/vtst', (req, res) =>{ //caso o caminho seja /vtst, mostrar as informa��es adquiridas
	  res.send('Recebido Mudan�a na Tomada su�te');
		console.log(req.query);
});

app.listen(3000, "0.0.0.0", function () { // Servidor ouvindo na porta 3000 em broadcast(qualquer usu�rio pode acessa-lo)
  console.log('servidor On na porta 3000!');
});
