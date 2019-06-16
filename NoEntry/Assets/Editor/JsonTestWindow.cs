using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using System.IO;

public class JsonTestWindow : EditorWindow
{
    [MenuItem("JSON/test")]
    static void Init(){
        JsonTestWindow window = (JsonTestWindow)EditorWindow.GetWindow(typeof(JsonTestWindow));
        window.Show();
    }

    void OnGUI(){
        if(GUILayout.Button("Save")){
            Save();
        }
    }

    void Save(){
        GameBalance[] data = new GameBalance[3];
        data[0] = new GameBalance();
        data[0].createMaxNum = 3;
        data[0].maxHp = 10;
        data[0].createTime = 5;
        data[0].monsterFireTime = 8;
        data[0].monsterRotationSpeed = 5;
        data[0].monsterDistance = 5;
        data[0].monsterColliderOffsetY = 3.25f;
        data[0].monsterColliderOffSizeY = 4.5f;

        data[1] = new GameBalance();
        data[1].createMaxNum = 4;
        data[1].maxHp = 10;
        data[1].createTime = 4;
        data[1].monsterFireTime = 5;
        data[1].monsterRotationSpeed = 7;
        data[1].monsterDistance = 10;
        data[1].monsterColliderOffsetY = 5.75f;
        data[1].monsterColliderOffSizeY = 9.5f;

        data[2] = new GameBalance();
        data[2].createMaxNum = 4;
        data[2].maxHp = 10;
        data[2].createTime = 3;
        data[2].monsterFireTime = 3;
        data[2].monsterRotationSpeed = 10;
        data[2].monsterDistance = 15;
        data[2].monsterColliderOffsetY = 8.25f;
        data[2].monsterColliderOffSizeY = 14.5f;

        DataHelper.ChangeData("gameBalance", data);
    }
}
