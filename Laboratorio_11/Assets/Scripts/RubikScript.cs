using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RubikScript : MonoBehaviour
{
    public float speed = 100.0f;
    public List<GameObject> cubes = new List<GameObject>(26);

    bool inputEnabled = true;
    Transform rubik;

    // Start is called before the first frame update
    void Start()
    {
        rubik = cubes[0].transform.parent;
    }

    // Update is called once per frame
    void Update()
    {
        if (!inputEnabled)
            return;

        float animDuration = 10.0f / speed;
        Vector3 rotationAxis;
        Func<Vector3, Vector3, bool> compPred;

        float orientation = (Input.GetKey(KeyCode.LeftShift) || Input.GetKey(KeyCode.RightShift)) ? -1.0f : 1.0f;
        float angleMag = (Input.GetKey(KeyCode.LeftControl) || Input.GetKey(KeyCode.RightControl)) ? 2.0f : 1.0f;

        if (Input.GetKey(KeyCode.R)) {
            rotationAxis = new Vector3(1, 0, 0);
            compPred = (Vector3 a, Vector3 b) => { return Convert.ToInt32(a.x) == b.x; };
            BuildRotationLayer(compPred, rotationAxis);
            StartCoroutine(RotateLayer(orientation * rotationAxis, 90.0f * angleMag, animDuration * angleMag));
        }

        else if (Input.GetKey(KeyCode.U)) {
            rotationAxis = new Vector3(0, 1, 0);
            compPred = (Vector3 a, Vector3 b) => { return Convert.ToInt32(a.y) == b.y; };
            BuildRotationLayer(compPred, rotationAxis);
            StartCoroutine(RotateLayer(orientation * rotationAxis, 90.0f * angleMag, animDuration * angleMag));
        }

        else if (Input.GetKey(KeyCode.F)) {
            rotationAxis = new Vector3(0, 0, -1);
            compPred = (Vector3 a, Vector3 b) => { return Convert.ToInt32(a.z) == b.z; };
            BuildRotationLayer(compPred, rotationAxis);
            StartCoroutine(RotateLayer(orientation * rotationAxis, 90.0f * angleMag, animDuration * angleMag));
        }

        else if (Input.GetKey(KeyCode.L)) {
            rotationAxis = new Vector3(-1, 0, 0);
            compPred = (Vector3 a, Vector3 b) => { return Convert.ToInt32(a.x) == b.x; };
            BuildRotationLayer(compPred, rotationAxis);
            StartCoroutine(RotateLayer(orientation * rotationAxis, 90.0f * angleMag, animDuration * angleMag));
        }

        else if (Input.GetKey(KeyCode.D)) {
            rotationAxis = new Vector3(0, -1, 0);
            compPred = (Vector3 a, Vector3 b) => { return Convert.ToInt32(a.y) == b.y; };
            BuildRotationLayer(compPred, rotationAxis);
            StartCoroutine(RotateLayer(orientation * rotationAxis, 90.0f * angleMag, animDuration * angleMag));
        }

        else if (Input.GetKey(KeyCode.B)) {
            rotationAxis = new Vector3(0, 0, 1);
            compPred = (Vector3 a, Vector3 b) => { return Convert.ToInt32(a.z) == b.z; };
            BuildRotationLayer(compPred, rotationAxis);
            StartCoroutine(RotateLayer(orientation * rotationAxis, 90.0f * angleMag, animDuration * angleMag));
        }
    }

    void BuildRotationLayer(Func<Vector3, Vector3, bool> pred, Vector3 layerPosition) {
        this.transform.position = layerPosition;
        foreach (var cube in cubes)
            if (pred(cube.transform.position, layerPosition))
                cube.transform.parent = this.transform;
    }

    IEnumerator RotateLayer(Vector3 rotationAxis, float angle, float duration) {
        inputEnabled = false;

        Quaternion from = transform.rotation;
        Quaternion to = Quaternion.AngleAxis(angle, rotationAxis) * from;

        float elapsedTime = 0.0f;
        while (elapsedTime < duration) {
            elapsedTime += Time.deltaTime;
            float t = elapsedTime / duration;
            transform.rotation = Quaternion.Slerp(from, to, t);
            yield return null;
        }

        transform.rotation = to;
        foreach (var cube in cubes)
            cube.transform.parent = rubik;

        inputEnabled = true;
    }
}
