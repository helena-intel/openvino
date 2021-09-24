# Getting Started with Demo Scripts and Demo Applications

## Demo Scripts

The demo scripts in `/opt/intel/openvino_2021/deployment_tools/demo` give you a starting point to learn the OpenVINO workflow. These scripts automatically perform the workflow steps to demonstrate running inference pipelines for different scenarios. The demo steps let you see how to: 
* Compile several samples from the source files delivered as part of the OpenVINO toolkit.
* Download trained models.
* Perform pipeline steps and see the output on the console.

> **NOTE**: You must have Internet access to run the demo scripts. If your Internet access is through a proxy server, make sure the operating system environment proxy information is configured.

The demo scripts can run inference on any [supported target device](https://software.intel.com/en-us/openvino-toolkit/hardware). Although the default inference device is CPU, you can use the `-d` parameter to change the inference device. The general command to run the scripts looks as follows:

```sh
./<script_name> -d [CPU, GPU, MYRIAD, HDDL]
```

Before running the demo applications on Intel® Processor Graphics or on an Intel® Neural Compute Stick 2 device, you must complete the additional configuration steps. For details, see:
* Steps for Intel® Processor Graphics (GPU) section in the [installation instructions](../install_guides/installing-openvino-linux.md)
* Steps for Intel® Neural Compute Stick 2 section in the [installation instructions](../install_guides/installing-openvino-linux.md).

The following paragraphs describe each demo script.

### Image Classification Demo Script
The `demo_squeezenet_download_convert_run` script illustrates the image classification pipeline.

The script: 
1. Downloads a SqueezeNet model. 
2. Runs the Model Optimizer to convert the model to the IR.
3. Builds the Image Classification Sample Async application.
4. Runs the compiled sample with the `car.png` image located in the `demo` directory.

**LINUX**

<details>
    <summary><strong>Click for an example of running the Image Classification demo script</strong></summary>

To preview the image that the script will classify:

```sh
cd ${INTEL_OPENVINO_DIR}/deployment_tools/demo
eog car.png
```

To run the script to perform inference on a CPU:

```sh
./demo_squeezenet_download_convert_run.sh
```

When the script completes, you see the label and confidence for the top-10 categories:

```sh

Top 10 results:

Image /home/user/dldt/inference-engine/samples/sample_data/car.png

classid probability label
------- ----------- -----
817     0.8363345   sports car, sport car
511     0.0946488   convertible
479     0.0419131   car wheel
751     0.0091071   racer, race car, racing car
436     0.0068161   beach wagon, station wagon, wagon, estate car, beach waggon, station waggon, waggon
656     0.0037564   minivan
586     0.0025741   half track
717     0.0016069   pickup, pickup truck
864     0.0012027   tow truck, tow car, wrecker
581     0.0005882   grille, radiator grille


total inference time: 2.6642941
Average running time of one iteration: 2.6642941 ms

Throughput: 375.3339402 FPS

[ INFO ] Execution successful
```

</details>

### Inference Pipeline Demo Script
The `demo_security_barrier_camera` uses vehicle recognition in which vehicle attributes build on each other to narrow in on a specific attribute.

The script:
1. Downloads three pre-trained model IRs.
2. Builds the Security Barrier Camera Demo application.
3. Runs the application with the downloaded models and the `car_1.bmp` image from the `demo` directory to show an inference pipeline. 

This application:

1. Identifies an object identified as a vehicle. 
2. Uses the vehicle identification as input to the second model, which identifies specific vehicle attributes, including the license plate.
3. Uses the the license plate as input to the third model, which recognizes specific characters in the license plate.

<details>
    <summary><strong>Click for an example of Running the Pipeline demo script</strong></summary>
    
To run the script performing inference on Intel® Processor Graphics:

```sh
./demo_security_barrier_camera.sh -d GPU
```

When the verification script completes, you see an image that displays the resulting frame with detections rendered as bounding boxes, and text:

![](../img/inference_pipeline_script_lnx.png)

</details>

### Benchmark Demo Script
The `demo_benchmark_app` script illustrates how to use the Benchmark Application to estimate deep learning inference performance on supported devices. 

The script: 
1. Downloads a SqueezeNet model.
2. Runs the Model Optimizer to convert the model to the IR.
3. Builds the Inference Engine Benchmark tool.
4. Runs the tool with the `car.png` image located in the `demo` directory.

<details>
    <summary><strong>Click for an example of running the Benchmark demo script</strong></summary>

To run the script that performs inference (runs on CPU by default):

```sh
./demo_benchmark_app.sh
```

When the verification script completes, you see the performance counters, resulting latency, and throughput values displayed on the screen.
</details>

## <a name="using-sample-application"></a>Demo/Sample Applications

This section guides you through a simplified workflow for the Intel® Distribution of OpenVINO™ toolkit using code samples and demo applications.
You will perform the following steps:
1. <a href="#download-models">Use the Model Downloader to download suitable models.</a>
2. <a href="#convert-models-to-intermediate-representation">Convert the models with the Model Optimizer.</a> 
3. <a href="download-media">Download media files to run inference on.</a>
4. <a href="run-image-classification">Run inference on the sample and see the results.</a>

### Build Samples

If samples have been manually built, skip this section. This step will take about 5-10 minutes, depending on your system.

**LINUX**

Build OpenVINO Demos
```
cd /opt/intel/openvino/inference_engine/demos
./build_demos.sh
```
Build OpenVINO Samples
```
cd /opt/intel/openvino/inference_engine/samples/cpp
./build_samples
```


### <a name="download-models"></a> Step 1: Download the Models

You must have a model that is specific for you inference task. Example model types are:
- Classification (AlexNet, GoogleNet, SqueezeNet, others) - Detects one type of element in a frame.
- Object Detection (SSD, YOLO) - Draws bounding boxes around multiple types of objects.
- Custom (Often based on SSD)

Options to find a model suitable for the OpenVINO™ toolkit are:
- Download public and Intel's pre-trained models from the [Open Model Zoo](https://github.com/openvinotoolkit/open_model_zoo) using [Model Downloader tool](@ref omz_tools_downloader).
- Download from GitHub*, Caffe* Zoo, TensorFlow* Zoo, etc.
- Train your own model.
        
This guide uses the Model Downloader to get pre-trained models. You can use one of the following options to find a model:

* **List the models available in the downloader**: 
```sh
cd /opt/intel/openvino_2021/deployment_tools/tools/model_downloader/
```
```sh
python3 info_dumper.py --print_all
```

* **Use `grep` to list models that have a specific name pattern**: 
```sh
python3 info_dumper.py --print_all | grep <model_name>
```

Use the Model Downloader to download the models to a models directory. This guide uses `<models_dir>` as the models directory and `<models_name>` as the model name:
```sh
sudo python3 ./downloader.py --name <model_name> --output_dir <models_dir>
```
> **NOTE:** Always run the downloader with `sudo`.

Download the following models if you want to run the Image Classification Sample and Security Barrier Camera Demo application:

|Model Name                                     | Code Sample or Demo App                             |
|-----------------------------------------------|-----------------------------------------------------|
|`squeezenet1.1`                                | Image Classification Sample                         |
|`vehicle-license-plate-detection-barrier-0106` | Security Barrier Camera Demo application            |
|`vehicle-attributes-recognition-barrier-0039`  | Security Barrier Camera Demo application            |
|`license-plate-recognition-barrier-0001`       | Security Barrier Camera Demo application            |

<details>
    <summary><strong>Click for an example of downloading the SqueezeNet Caffe* model</strong></summary>

To download the SqueezeNet 1.1 Caffe* model to the `~/models` folder:

```sh
sudo python3 ./downloader.py --name squeezenet1.1 --output_dir ~/models
```

Your screen looks similar to this after the download:
```
###############|| Downloading models ||###############

========= Downloading /home/username/models/public/squeezenet1.1/squeezenet1.1.prototxt

========= Downloading /home/username/models/public/squeezenet1.1/squeezenet1.1.caffemodel
... 100%, 4834 KB, 3157 KB/s, 1 seconds passed

###############|| Post processing ||###############

========= Replacing text in /home/username/models/public/squeezenet1.1/squeezenet1.1.prototxt =========
```
</details>

<details>
    <summary><strong>Click for an example of downloading models for the Security Barrier Camera Demo application</strong></summary>

To download all three pre-trained models in FP16 precision to the `~/models` folder:   

```sh
./downloader.py --name vehicle-license-plate-detection-barrier-0106,vehicle-attributes-recognition-barrier-0039,license-plate-recognition-barrier-0001 --output_dir ~/models --precisions FP16
```   
Your screen looks similar to this after the download:
```
################|| Downloading models ||################

========== Downloading /home/username/models/intel/vehicle-license-plate-detection-barrier-0106/FP16/vehicle-license-plate-detection-barrier-0106.xml
... 100%, 204 KB, 183949 KB/s, 0 seconds passed

========== Downloading /home/username/models/intel/vehicle-license-plate-detection-barrier-0106/FP16/vehicle-license-plate-detection-barrier-0106.bin
... 100%, 1256 KB, 3948 KB/s, 0 seconds passed

========== Downloading /home/username/models/intel/vehicle-attributes-recognition-barrier-0039/FP16/vehicle-attributes-recognition-barrier-0039.xml
... 100%, 32 KB, 133398 KB/s, 0 seconds passed

========== Downloading /home/username/models/intel/vehicle-attributes-recognition-barrier-0039/FP16/vehicle-attributes-recognition-barrier-0039.bin
... 100%, 1222 KB, 3167 KB/s, 0 seconds passed

========== Downloading /home/username/models/intel/license-plate-recognition-barrier-0001/FP16/license-plate-recognition-barrier-0001.xml
... 100%, 47 KB, 85357 KB/s, 0 seconds passed

========== Downloading /home/username/models/intel/license-plate-recognition-barrier-0001/FP16/license-plate-recognition-barrier-0001.bin
... 100%, 2378 KB, 5333 KB/s, 0 seconds passed

################|| Post-processing ||################
```

</details>  

### Step 2: Convert the Model with Model Optimizer

In this step, your trained models are ready to run through the Model Optimizer to convert them to the Intermediate Representation format. This is required before using the Inference Engine with the model.

Models in the IR format always include an `.xml` and `.bin` file and may also include other files such as `.json` or `.mapping`. Make sure you have these files in the same directory for the Inference Engine to find them.

REQUIRED: `model_name.xml` REQUIRED: `model_name.bin` OPTIONAL: `model_name.json`,  `model_name.mapping`, etc.

This guide uses the public SqueezeNet 1.1 Caffe* model to run the Image Classification Sample. See the example to download a model in the Download Models section to learn how to download this model.

The SqueezeNet1.1 model is downloaded in the Caffe* format. You must use the Model Optimizer to convert the model to the IR. The `vehicle-license-plate-detection-barrier-0106`, `vehicle-attributes-recognition-barrier-0039`, `license-plate-recognition-barrier-0001` models are downloaded in the Intermediate Representation format. You don't need to use the Model Optimizer to covert these models.

Create an `<ir_dir>` directory to contain the model's Intermediate Representation (IR).

```
mkdir ~/ir
```

The Inference Engine can perform inference on different precision formats, such as FP32, FP16, INT8. To prepare an IR with specific precision, run the Model Optimizer with the appropriate `--data_type` option.

**LINUX**:

Run the Model Optimizer Script:

```
cd /opt/intel/openvino/deployment_tools/model_optimizer
```

`python3 ./mo_py --input_model <model_dir>/<model_file> --data_type <model_precision> --output_dir <ir_dir>`produced IR files are in the <ir_dir> directory.

The actual command:

```
python3 ./mo.py --input_model ~/models/public/squeezenet1.1/squeezenet1.1.caffemodel --data_type FP16 --output_dir ~/models/public/squeezenet1.1/ir
```

### <a name="download-media"></a> Step 3: Download a Video or a Still Photo as Media

Many sources are available from which you can download video media to use the code samples and demo applications. Possibilities include: 
- https://videos.pexels.com
- https://images.google.com

As an alternative, the Intel® Distribution of OpenVINO™ toolkit includes two sample images that you can use for running code samples and demo applications:
* `/opt/intel/openvino_2021/deployment_tools/demo/car.png`
* `/opt/intel/openvino_2021/deployment_tools/demo/car_1.bmp`

### <a name="run-image-classification"></a>Step 4: Run Inference on the Sample

> **NOTE**: The Image Classification code sample is automatically compiled when you ran the Image Classification demo script. If you want to compile it manually, see the *Build the Sample Applications on Linux* section in the [Inference Engine Code Samples Overview](../IE_DG/Samples_Overview.md). 

To run the **Image Classification** code sample with an input image on the IR: 

1. Set up the OpenVINO environment variables:
   ```sh
   source /opt/intel/openvino_2021/bin/setupvars.sh
   ``` 
2. Go to the code samples build directory:
   ```sh
   cd ~/inference_engine_samples_build/intel64/Release
   ```
3. Run the code sample executable, specifying the input media file, the IR of your model, and a target device on which you want to perform inference:
   ```sh
   classification_sample_async -i <path_to_media> -m <path_to_model> -d <target_device>
   ```
<details>
    <summary><strong>Click for examples of running the Image Classification code sample on different devices</strong></summary>

The following commands run the Image Classification Code Sample using the `car.png` file from the `/opt/intel/openvino_2021/deployment_tools/demo/` directory as an input image, the IR of your model from `~/models/public/squeezenet1.1/ir` and on different hardware devices:

**CPU:**
   ```sh
   ./classification_sample_async -i /opt/intel/openvino_2021/deployment_tools/demo/car.png -m ~/models/public/squeezenet1.1/ir/squeezenet1.1.xml -d CPU
   ```

   **GPU:**
   
   > **NOTE**: Running inference on Intel® Processor Graphics (GPU) requires additional hardware configuration steps. For details, see the Steps for Intel® Processor Graphics (GPU) section in the [installation instructions](../install_guides/installing-openvino-linux.md).
   ```sh
   ./classification_sample_async -i /opt/intel/openvino_2021/deployment_tools/demo/car.png -m ~/models/public/squeezenet1.1/ir/squeezenet1.1.xml -d GPU
   ```
   
   **MYRIAD:** 

   > **NOTE**: Running inference on VPU devices (Intel® Neural Compute Stick 2) with the MYRIAD plugin requires additional hardware configuration steps. For details, see the Steps for Intel® Neural Compute Stick 2 section in the [installation instructions](../install_guides/installing-openvino-linux.md).
   ```sh   
   ./classification_sample_async -i /opt/intel/openvino_2021/deployment_tools/demo/car.png -m ~/models/public/squeezenet1.1/ir/squeezenet1.1.xml -d MYRIAD
   ```
   
   **HDDL:**

  > **NOTE**: Running inference on the Intel® Vision Accelerator Design with Intel® Movidius™ VPUs device with the HDDL plugin requires additional hardware configuration steps. For details, see the Steps for Intel® Vision Accelerator Design with Intel® Movidius™ VPUs section in the [installation instructions](../install_guides/installing-openvino-linux.md).
  ```sh   
  ./classification_sample_async -i /opt/intel/openvino_2021/deployment_tools/demo/car.png -m ~/models/public/squeezenet1.1/ir/squeezenet1.1.xml -d HDDL
  ```

When the Sample Application completes, you see the label and confidence for the top-10 categories on the display. Below is a sample output with inference results on CPU:    
```sh
Top 10 results:

Image /home/user/dldt/inference-engine/samples/sample_data/car.png

classid probability label
------- ----------- -----
817     0.8363345   sports car, sport car
511     0.0946488   convertible
479     0.0419131   car wheel
751     0.0091071   racer, race car, racing car
436     0.0068161   beach wagon, station wagon, wagon, estate car, beach waggon, station waggon, waggon
656     0.0037564   minivan
586     0.0025741   half track
717     0.0016069   pickup, pickup truck
864     0.0012027   tow truck, tow car, wrecker
581     0.0005882   grille, radiator grille


total inference time: 2.6642941
Average running time of one iteration: 2.6642941 ms

Throughput: 375.3339402 FPS

[ INFO ] Execution successful
```

</details>

## Exercises

The following series of exercises guide you through using samples of increasing complexity. As you move through each exercise you will get a sense of how to use OpenVINO™ in more sophisticated use cases.

In these exercises, you will:
1. Convert and optimize a neural network model to work on Intel® hardware.
2. Run computer vision applications using optimized models and appropriate media.
   -During optimization with the DL Workbench™, a subset of ImageNet* and VOC* images are used.
   -When running samples, we'll use either an image or video file located on this system.
> **NOTE**: Before starting these sample exercises, change directories into the samples directory:
> ```
> cd ~/omz_demos_build/intel64/Release
> ```


> **NOTE**: During this exercise you will move to multiple directories and occasionally copy files so that you don't have to specify full paths in commands. You are welcome to set up environment variables to make these tasks easier, but we leave that to you.


> **REMEMBER**: When using OpenVINO™ from the command line, you must set up your environment whenever you change users or launch a new terminal.
> ```
> source /opt/intel/openvino/bin/setupvars.sh
> ```


### Exercise 1: Run A Sample Application
Convert a model using the Model Optimizer then use a sample application to load the model and run inference.

In this section, you will convert an FP32 model suitable for running on a CPU.

**Prepare the Software Environment**

1. Set up the environment variables when logging in, changing users, or launching a new terminal. (Detail above.)
2. Make a destination directory for the FP32 SqueezeNet* Model:
```
mkdir ~/squeezenet1.1_FP32
cd ~/squeezenet1.1_FP32
```
*\*Convert and Optimize a Neural Network Model from Caffe*

Use the Model Optimizer to convert an FP32 SqueezeNet* Caffe* model into an optimized Intermediate Representation (IR):
```
python3 /opt/intel/openvino/deployment_tools/model_optimizer/mo.py --input_model ~/openvino_models/models/public/squeezenet1.1/squeezenet1.1.caffemodel --data_type FP32 --output_dir .
```
**Prepare the Data (Media) or Dataset**

> **NOTE**: In this case, it's a single image.

1. Copy the labels file to the same location as the IR model.
```
cp /opt/intel/openvino/deployment_tools/demo/squeezenet1.1.labels .
```
   - Tip: The labels file contains the classes used by this SqueezeNet* model.
   - If it's is in the same directory as the model, the inference results will show text in addition to confidence percentages.
  
2. Copy a sample image to the current directory. You will use this with your optimized model:
```
sudo cp /opt/intel/openvino/deployment_tools/demo/car.png .
```

**Run the Sample Application**

Once your setup is complete, you're ready to run a sample application:
```
~/inference_engine_samples_build/intel64/Release/classification_sample_async -i car.png -m ~/squeezenet1.1_FP32/squeezenet1.1.xml -d CPU
```

> **NOTE**: You can usually see an application's help information (parameters, etc.) by using the --h option.
> ```
> ~/inference_engine_samples_build/intel64/Release/classification_sample_async -h
> ```


If desired, you can look at the original image using the Eye of Gnome application (installed by default on Ubuntu systems):
```
eog car.png
```


## Other Demos/Samples

For more samples and demos, you can visit the samples and demos pages. You can review samples and demos by complexity or by usage.

[Samples](../IE_DG/Samples_Overview.md)

[Demos](@ref omz_demos)
