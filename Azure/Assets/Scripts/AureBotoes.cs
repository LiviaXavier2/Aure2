using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class AureBotoes : MonoBehaviour
{
    // Funcao chamada quando um botao é clicado
    public void ChangeScene(string sceneName)
    {
        // Carrega a cena com o nome especificado
        SceneManager.LoadScene(sceneName);
    }
}
