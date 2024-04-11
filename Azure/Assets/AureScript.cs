using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class AureScript : MonoBehaviour
{
    public string BotaoLogin;
    public string BotaoCriarConta;
    public string BotaoSenha;

    public void LoadSceneBotaoLogin()
    {
        // Carregar a cena do login
        SceneManager.LoadScene(BotaoLogin);
    }

    public void LoadSceneBotaoCriarConta()
    {
        // Carregar a cena para criar uma conta
        SceneManager.LoadScene(BotaoCriarConta);
    }

    public void LoadSceneBotaoSenha()
    {
        // Carregar a cena para recuperar senha
        SceneManager.LoadScene(BotaoSenha);
    }
}
