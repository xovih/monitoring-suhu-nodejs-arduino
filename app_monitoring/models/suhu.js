const Mongoose = require("mongoose");
const moment = require('moment-timezone');
let waktuIndo = moment.tz(Date.now(), "Asia/Jakarta");

const SuhuSchema = new Mongoose.Schema({
    waktu: {type: Date, default: waktuIndo},
    suhuc: Number,
    suhuf: Number,
    lembab: Number
});

module.exports = Mongoose.model('suhuteika', SuhuSchema);