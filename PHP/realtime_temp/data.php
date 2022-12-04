<?php 
    require("Liberary/phpMQTT.php");

    $host   = "riset.revolusi-it.com";
    $port     = 1883;
    $username = "G.211.19.0079";
    $password = "";
    $mqtt = new bluerhinos\phpMQTT($host, $port, "ClientID".rand());

    $suhu = "0";
    $kelembapan = "0";
    $getSuhu = false;
    $getKelembapan = false;

    if(!$mqtt->connect(true,NULL,$username,$password)) {
        exit(1);
    }

    // subscribe ke topik
    $topics['iot/suhu79'] = array("qos"=>0, "function"=>"setSuhu");
    $mqtt->subscribe($topics,0);
    $topics2['iot/kelembapan79'] = array("qos"=>0, "function"=>"setKelembapan");
    $mqtt->subscribe($topics2,0);

    while (true)
    {
        $mqtt->proc();
        if ($getSuhu && $getKelembapan)
        {
            echo json_encode(array(
                "suhu" => $suhu,
                "kelembapan" => $kelembapan
            ));
            break;
        }
    }
    
    $mqtt->close();

    function setSuhu($topic,$msg) {
        global $suhu,  $getSuhu;
        $suhu = $msg;
        $getSuhu = true;
    }
    function setKelembapan($topic,$msg) {
        global $kelembapan, $getKelembapan;
        $kelembapan = $msg;
        $getKelembapan = true;
    }   
?>