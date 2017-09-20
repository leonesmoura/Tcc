var express = require('express'); //Framework para criar o servidor
var request = require('request'); //Biblioteca para fazer requisição dos ESPs

var app = express();

// Adicionando Métodos Http.
app.use(function (req, res, next) {
    res.setHeader('Access-Control-Allow-Origin', '*');
    res.setHeader('Access-Control-Allow-Methods', 'GET, POST, OPTIONS, PUT, PATCH, DELETE');
    res.setHeader('Access-Control-Allow-Headers', 'X-Requested-With,content-type');
    res.setHeader('Access-Control-Allow-Credentials', true);
    next();
});

app.use('/', express.static('public')); //Serve a página para os usuários. Deve haver uma pasta com o nome "public" no mesmo local do servidor e nela conter a página html

app.get('/vls1', (req, res) =>{ //caso o caminho seja /vls1, mostrar as informações adquiridas
	  res.send('Recebido Mudança na Lâmpada Sala 1');
		console.log(req.query);
});
 
app.get('/vls2', (req, res) =>{ //caso o caminho seja /vls2, mostrar as informações adquiridas
	  res.send('Recebido Mudança na Lâmpada Sala 2');
		console.log(req.query);
});
 
app.get('/vlv', (req, res) =>{ //caso o caminho seja /vlv, mostrar as informações adquiridas
	  res.send('Recebido Mudança na Lâmpada Varanda');
		console.log(req.query);
});
 
app.get('/vlq1', (req, res) =>{ //caso o caminho seja /vlq1, mostrar as informações adquiridas
     res.send('Recebido Mudança na Lâmpada Quarto 1');
	  console.log(req.query);
});
 
app.get('/vlq2', (req, res) =>{ //caso o caminho seja /vlq2, mostrar as informações adquiridas
      res.send('Recebido Mudança na Lâmpada Quarto 2');
      console.log(req.query);
});
 
app.get('/vlc', (req, res) =>{ //caso o caminho seja /vlc, mostrar as informações adquiridas
	  res.send('Recebido Mudança na Lâmpada Cozinha');
		console.log(req.query);
});

app.get('/vlb', (req, res) =>{ //caso o caminho seja /vlb, mostrar as informações adquiridas
	  res.send('Recebido Mudança na Lâmpada banheiro');
		console.log(req.query);
});

app.get('/vlst', (req, res) =>{ //caso o caminho seja /vlst, mostrar as informações adquiridas
	  res.send('Recebido Mudança na Lâmpada Suíte');
		console.log(req.query);
});

app.get('/vlas', (req, res) =>{ //caso o caminho seja /vlas, mostrar as informações adquiridas
	  res.send('Recebido Mudança na Lâmpada Área de Serviço');
		console.log(req.query);
});
 
app.get('/vtq1', (req, res) =>{ //caso o caminho seja /vtq1, mostrar as informações adquiridas
	  res.send('Recebido Mudança na Tomada Quarto 1');
		console.log(req.query);
});

app.get('/vtq2', (req, res) =>{ //caso o caminho seja /vtq2, mostrar as informações adquiridas
	  res.send('Recebido Mudança na Tomada Quarto 2');
		console.log(req.query);
});

app.get('/vtst', (req, res) =>{ //caso o caminho seja /vtst, mostrar as informações adquiridas
	  res.send('Recebido Mudança na Tomada suíte');
		console.log(req.query);
});

app.listen(3000, "0.0.0.0", function () { // Servidor ouvindo na porta 3000 em broadcast(qualquer usuário pode acessa-lo)
  console.log('servidor On na porta 3000!');
});
