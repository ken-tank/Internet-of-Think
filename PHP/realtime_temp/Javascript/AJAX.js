function reqListener () {
    console.log(this.responseText);
}

function getData() {
    var oReq = new XMLHttpRequest();
    oReq.onload = function() {
        var obj = JSON.parse(this.responseText);
        document.getElementById("suhu").innerHTML = obj.suhu.toString();
        document.getElementById("kelembapan").innerHTML = obj.kelembapan.toString();
    };

    oReq.open("get", "data.php", true);

    oReq.send();

    setTimeout( () => {
        getData();
    }, 1000);
}

getData();
