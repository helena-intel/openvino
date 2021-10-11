# OpenVINO™ Learning Path

## Overview

 Introductory paragraph text here

## Toolkit Components

 ![](../img/ie_workflow_steps.png)
 
 The basic steps to use the software are as follows:
 1. **Get a trained model** for your inference task. Example inference tasks: pedestrian detection, face detection, vehicle detection, license plate recognition, head pose.
2. **Run the trained model through the Model Optimizer** to convert the model to the Intermediate Representation (IR) format, which consists of a pair of .xml and .bin files that are used as the input for Inference Engine.
3. **Use the Inference Engine API** in an application to run inference with your media against the IR (optimized model) and output inference results. The application can be an OpenVINO™ sample or demo, it can be your own application that uses the Inference Engine.

THe Intel® Distribution of OpenVINO™ Toolkit includes the following components:
- Deep Learning Model Optimizer: A cross-platform command-line tool for importing models and preparing them for optimal execution with the Inference Engine. The Model Optimizer imports, converts, and optimizes models, which were trained in popular frameworks, such as Caffe*, TensorFlow*, MXNet*, Kaldi*, and ONNX*.
- Deep Learning Inference Engine: A unified API to allow high performance inference on many hardware types including Intel® CPU, Intel® Integrated Graphics, Intel® Neural Compute Stick 2, Intel® Vision Accelerator Design with Intel® Movidius™ vision processing unit (VPU).
- Inference Engine Samples: A set of simple console applications demonstrating how to use the Inference Engine in your applications.
- Deep Learning Workbench: A web-based graphical environment that allows you to easily use various sophisticated OpenVINO™ toolkit components.
- Post-training Optimization Tool: A tool to calibrate a model and then execute it in the INT8 precision.
- Additional Tools: A set of tools to work with your models including Benchmark App, Cross Check Tool, Compile tool.
- Open Model Zoo 
   - Demos: Console applications that provide robust application templates to help you implement specific deep learning scenarios.
   - Additional Tools: A set of tools to work with your models including Accuracy Checker Utility and Model Downloader.
   - Documentation for Pretrained Models: Documentation for pre-trained models that are available in the Open Model Zoo repository.
- Deep Learning Streamer (DL Streamer): Streaming analytics framework, based on GStreamer, for constructing graphs of media analytics components. DL Streamer can be installed by the Intel® Distribution of OpenVINO™ toolkit installer. Its open-source version is available on GitHub. For the DL Streamer documentation, see:
   - DL Streamer Samples
   - API Reference
   - Elements
   - Tutorial
- OpenCV : OpenCV* community version compiled for Intel® hardware
- Intel® Media SDK (in Intel® Distribution of OpenVINO™ toolkit for Linux only)
 

## OpenVINO™ Toolkit Directory Structure

**LINUX tab goes here**

This guide assumes you have completed all Intel® Distribution of OpenVINO™ toolkit installation and configuration steps. If you have not yet installed and configured the toolkit, see [Install Intel® Distribution of OpenVINO™ toolkit for Linux*](../install_guides/installing-openvino-linux.md).

By default, the Intel® Distribution of OpenVINO™ is installed to one of the following directories, referred to as `<INSTALL_DIR>` in the documentation or with the environment variable $INTEL_OPENVINO_DIR:
* For root or administrator: `/opt/intel/openvino_<version>/`
* For regular users: `/home/<USER>/intel/openvino_<version>/`

For simplicity, a symbolic link to the latest installation is also created: `/opt/intel/openvino_2021/` or `/home/<user>/intel/openvino_2021/`

If you installed the Intel® Distribution of OpenVINO™ toolkit to a directory other than the default, replace `/opt/intel` or `/home/<USER>/` in documentation steps with the directory in which you installed the software.

The primary tools for deploying your models and applications are in the `/opt/intel/openvino_2021/deployment_tools` directory.
<details>
    <summary><strong>Click for the Intel® Distribution of OpenVINO™ toolkit directory structure</strong></summary>
   

| Directory&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; | Description                                                                           |  
|:----------------------------------------|:--------------------------------------------------------------------------------------|
| `demo/`                                 | Demo scripts. Demonstrate pipelines for inference scenarios, automatically perform steps and print detailed output to the console. For more information, see the [Use OpenVINO: Demo Scripts](#use-openvino-demo-scripts) section.|
| `inference_engine/`                     | Inference Engine directory. Contains Inference Engine API binaries and source files, samples and extensions source files, and resources like hardware drivers.|
| `~intel_models/` | Symbolic link to the `intel_models` subfolder of the `open_model-zoo` folder |
| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`include/`      | Inference Engine header files. For API documentation, see the [Inference Engine API Reference](./annotated.html). |
| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`lib/`          | Inference Engine binaries.|
| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`samples/`      | Inference Engine samples. Contains source code for C++ and Python* samples and build scripts. See the [Inference Engine Samples Overview](../IE_DG/Samples_Overview.md). |
| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`src/`          | Source files for CPU extensions.|
| `model_optimizer/`                      | Model Optimizer directory. Contains configuration scripts, scripts to run the Model Optimizer and other files. See the [Model Optimizer Developer Guide](../MO_DG/Deep_Learning_Model_Optimizer_DevGuide.md).
| `open_model_zoo/`                       | Open Model Zoo directory. Includes the Model Downloader tool to download [pre-trained OpenVINO](@ref omz_models_group_intel) and public models, OpenVINO models documentation, demo applications and the Accuracy Checker tool to evaluate model accuracy.|
| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`demos/`        | Demo applications for inference scenarios. Also includes documentation and build scripts.| 
| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`intel_models/` | Pre-trained OpenVINO models and associated documentation. See the [Overview of OpenVINO™ Toolkit Pre-Trained Models](@ref omz_models_group_intel).|
| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`tools/`        | Model Downloader and Accuracy Checker tools. |
| `tools/`                                | Contains a symbolic link to the Model Downloader folder and auxiliary tools to work with your models: Calibration , Benchmark and Collect Statistics tools.|

</details>

**macOS tab goes here**

This guide assumes you completed all Intel® Distribution of OpenVINO™ toolkit installation and configuration steps. If you have not yet installed and configured the toolkit, see [Install Intel® Distribution of OpenVINO™ toolkit for macOS*](../install_guides/installing-openvino-macos.md).

By default, the Intel® Distribution of OpenVINO™ is installed to one of the following directories, referred to as `<INSTALL_DIR>` in the documentation or with the environment variable $INTEL_OPENVINO_DIR:
* For root or administrator: `/opt/intel/openvino_<version>/`
* For regular users: `/home/<USER>/intel/openvino_<version>/`

For simplicity, a symbolic link to the latest installation is also created: `/opt/intel/openvino_2021/` or `/home/<user>/intel/openvino_2021/`

If you installed the Intel® Distribution of OpenVINO™ toolkit to a directory other than the default, replace `/opt/intel` or `/home/<USER>/` when following documentation steps with the directory in which you installed the software. 

The primary tools for deploying your models and applications are installed to the `<INSTALL_DIR>/deployment_tools` directory.
<details>
    <summary><strong>Click for the Intel® Distribution of OpenVINO™ toolkit directory structure</strong></summary>
   

| Directory&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; | Description                                                                           |  
|:----------------------------------------|:--------------------------------------------------------------------------------------|
| `demo/`                                 | Demo scripts. Demonstrate pipelines for inference scenarios, automatically perform steps and print detailed output to the console. For more information, see the [Use OpenVINO: Demo Scripts](#use-openvino-demo-scripts) section.|
| `inference_engine/`                     | Inference Engine directory. Contains Inference Engine API binaries and source files, samples and extensions source files, and resources like hardware drivers.|
| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`external/`     | Third-party dependencies and drivers.|
| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`include/`      | Inference Engine header files. For API documentation, see the [Inference Engine API Reference](./annotated.html). |
| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`lib/`          | Inference Engine static libraries.|
| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`samples/`      | Inference Engine samples. Contains source code for C++ and Python* samples and build scripts. See the [Inference Engine Samples Overview](../IE_DG/Samples_Overview.md). |
| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`share/`        | CMake configuration files for linking with Inference Engine.|
| `~intel_models/` | Symbolic link to the `intel_models` subfolder of the `open_model_zoo` folder.|
| `model_optimizer/`                      | Model Optimizer directory. Contains configuration scripts, scripts to run the Model Optimizer and other files. See the [Model Optimizer Developer Guide](../MO_DG/Deep_Learning_Model_Optimizer_DevGuide.md).|
| `ngraph/`                               | nGraph directory. Includes the nGraph header and library files. |
| `open_model_zoo/`                       | Open Model Zoo directory. Includes the Model Downloader tool to download [pre-trained OpenVINO](@ref omz_models_group_intel) and public models, OpenVINO models documentation, demo applications and the Accuracy Checker tool to evaluate model accuracy.|
| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`demos/`        | Demo applications for inference scenarios. Also includes documentation and build scripts.| 
| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`intel_models/` | Pre-trained OpenVINO models and associated documentation. See the [Overview of OpenVINO™ Toolkit Pre-Trained Models](@ref omz_models_group_intel).|
| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`models`        | Intel's trained and public models that can be obtained with Model Downloader.|
| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`tools/`        | Model Downloader and Accuracy Checker tools. |
| `tools/`                                | Contains a symbolic link to the Model Downloader folder and auxiliary tools to work with your models: Calibration tool, Benchmark and Collect Statistics tools.|

</details>

**WINDOWS**

This guide assumes you completed all Intel® Distribution of OpenVINO™ toolkit installation and configuration steps. If you have not yet installed and configured the toolkit, see [Install Intel® Distribution of OpenVINO™ toolkit for Windows*](../install_guides/installing-openvino-windows.md).

By default, the installation directory is `C:\Program Files (x86)\Intel\openvino_<version>`, referred to as `<INSTALL_DIR>`. If you installed the Intel® Distribution of OpenVINO™ toolkit to a directory other than the default, replace `C:\Program Files (x86)\Intel` with the directory in which you installed the software. For simplicity, a shortcut to the latest installation is also created: `C:\Program Files (x86)\Intel\openvino_2021`.

The primary tools for deploying your models and applications are installed to the `<INSTALL_DIR>\deployment_tools` directory.
<details>
    <summary><strong>Click for the <code>deployment_tools</code> directory structure</strong></summary>
   

| Directory&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; | Description                                                                           |  
|:----------------------------------------|:--------------------------------------------------------------------------------------|
| `demo\`                                 | Demo scripts. Demonstrate pipelines for inference scenarios, automatically perform steps and print detailed output to the console. For more information, see the [Use OpenVINO: Demo Scripts](#use-openvino-demo-scripts) section.|
| `inference_engine\`                     | Inference Engine directory. Contains Inference Engine API binaries and source files, samples and extensions source files, and resources like hardware drivers.|
| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`bin\`          | Inference Engine binaries.|
| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`external\`     | Third-party dependencies and drivers.|
| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`include\`      | Inference Engine header files. For API documentation, see the [Inference Engine API Reference](./annotated.html). |
| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`lib\`          | Inference Engine static libraries.|
| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`samples\`      | Inference Engine samples. Contains source code for C++ and Python* samples and build scripts. See the [Inference Engine Samples Overview](../IE_DG/Samples_Overview.md). |
| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`share\`        | CMake configuration files for linking with Inference Engine.|
| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`src\`          | Source files for CPU extensions.|
| `~intel_models\` | Symbolic link to the `intel_models` subfolder of the `open_model_zoo` folder. |
| `model_optimizer\`                      | Model Optimizer directory. Contains configuration scripts, scripts to run the Model Optimizer and other files. See the [Model Optimizer Developer Guide](../MO_DG/Deep_Learning_Model_Optimizer_DevGuide.md). |
| `ngraph\`                               | nGraph directory. Includes the nGraph header and library files. |   
| `open_model_zoo\`                       | Open Model Zoo directory. Includes the Model Downloader tool to download [pre-trained OpenVINO](@ref omz_models_group_intel) and public models, OpenVINO models documentation, demo applications and the Accuracy Checker tool to evaluate model accuracy.|
| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`demos\`        | Demo applications for inference scenarios. Also includes documentation and build scripts.| 
| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`intel_models\` | Pre-trained OpenVINO models and associated documentation. See the [Overview of OpenVINO™ Toolkit Pre-Trained Models](@ref omz_models_group_intel).|
| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`models`        | Intel's trained and public models that can be obtained with Model Downloader.|
| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`tools\`        | Model Downloader and Accuracy Checker tools. |
| `tools\`                                | Contains a symbolic link to the Model Downloader folder and auxiliary tools to work with your models: Calibration tool, Benchmark and Collect Statistics tools.|

</details>

## Get Started with Demo Scripts and Samples

The [Get Started page](../get_started.md) walks you through the exercises that are an introduction to using OpenVINO to run samples and demos, and a quick introduction to basic tools such as the Model Downloader and Model Optimizer. The exercises are intended to become more difficult as you progress. Early exercises provide step-by-step instructions, while later exercises give more general guidance. The goal is to teach developers the concepts and skills needed to use all of the resources available for OpenVINO™.

## Additional Resources 

[**OpenVINO toolkit Deployment Tools**](http://google.com/)
