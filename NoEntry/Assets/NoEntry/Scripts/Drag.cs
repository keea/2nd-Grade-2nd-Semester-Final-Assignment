using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Drag : MonoBehaviour
{
    float OnMouseDonwTime = 0.0f;
    private void OnMouseDrag()
    {
        Debug.Log(OnMouseDonwTime);
        OnMouseDonwTime += Time.deltaTime;

        Vector2 mousePosition = new Vector2(Input.mousePosition.x,
            Input.mousePosition.y);

        Vector2 objPostion = Camera.main.ScreenToWorldPoint(mousePosition);
        transform.position = objPostion;
    }

    //마우스 뗄 떼 초기화.
    private void OnMouseUp()
    {
        OnMouseDonwTime = 0.0f;
    }
}
