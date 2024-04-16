using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Android.BLE;
using Android.BLE.Commands;
using UnityEngine.Android;
using UnityEngine.UI;
using System.Text;
using TMPro;
using UnityEngine.SceneManagement;

public class BleInterador : MonoBehaviour
{
    [SerializeField]
    private Button botaoConectar, botaoDesconectar;
    [SerializeField]
    TextMeshProUGUI status;
    [SerializeField]
    private string _servico = "0000ffe0-0000-1000-8000-00805f9b34fb",
                   _caracteristicaRX = "0000ffe1-0000-1000-8000-00805f9b34fb", 
                   _caracteristicaTX = "0000ffe2-0000-1000-8000-00805f9b34fb";
    [SerializeField]
    string nomeDispositivo;


    //aqui
    [SerializeField]
    private int _scanTime = 10;




    private ConnectToDevice _connectCommand;



    IConexao conexao;

    private void Start()
    {
#if !UNITY_EDITOR
        conexao = new BleInteradorAndroid(status, _scanTime,nomeDispositivo,_servico,_caracteristicaTX,_caracteristicaRX,Receber, botaoConectar,botaoDesconectar,null,null);
#endif
#if UNITY_EDITOR
        conexao = new BleInteradorWin(status, _scanTime, nomeDispositivo, _servico, _caracteristicaTX,_caracteristicaRX,Receber, botaoConectar, botaoDesconectar, null, null);
#endif
        botaoConectar.interactable = true;
        botaoDesconectar.interactable = false;
    }
#if UNITY_EDITOR
    private void OnApplicationQuit()
    {
        BleApi.Quit();
    }
#endif


    public void ScanForDevices()
    {
       // StartCoroutine
        conexao.OnScan();
    }

    public void DisconnectDevice()
    {
        conexao.OnDesconectar();//desconecta o device     
    }

    private void Update()
    {
        conexao.Update_Unity();
    }

    private void OnDisconnected(string deviceUuid)
    {
        _connectCommand.End();
    }

    public void EnviarOn()
    {
        conexao.Enviar("on\n");
    }
    public void EnviarOff()
    {
        conexao.Enviar("off\n");
    }

    public void Receber(string dados)
    {
        status.text = dados;
    }
}

//Programação da UI aqui - botões
public class SceneTransition
{
    // Função chamada quando um botão é clicado
    public void ChangeScene(string sceneName)
    {
        // Carrega a cena com o nome especificado
        SceneManager.LoadScene(sceneName);
    }
}

