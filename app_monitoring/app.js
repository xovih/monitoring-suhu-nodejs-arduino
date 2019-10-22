const Express = require("express");
const Mongoose = require("mongoose");
const BodyParser = require("body-parser");
const Ejs = require("ejs");
const Engine = require("ejs-mate");
const Path = require("path");
const Suhu = require("./models/suhu");
const DbConfig = require('./config/database');


const App = new Express();
const MyPort = 2810;

// Setup DB
Mongoose.connect(DbConfig.dbserver, DbConfig.mongoset, function(err){
    if (err) {
        console.log(err);
    } else {
        console.log(`Connected to database server`);
    }
});

// Middle Ware
App.set('views', Path.join(__dirname, 'views'));
App.set('view engine', 'ejs');
App.use(Express.static(Path.join(__dirname, 'public')));
App.use(BodyParser.json());
App.use(BodyParser.urlencoded({extended:true}));

// Server Setup
App.listen(MyPort, function(err){
    if (err) console.log(err);
    console.log(`Server running at port : ${MyPort}`);
});

App.post('/insert-suhu', function(req, res, next){
    let suhu = new Suhu();
    suhu.suhuc = req.body.suhuc;
    suhu.suhuf = req.body.suhuf;
    suhu.lembab = req.body.kelembaban;
    suhu.save(function(err){
        if (err) console.log(err);
        res.json(suhu);
    });
});

