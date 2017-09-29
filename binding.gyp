{
    "targets": [
        {
            "target_name": "aminonative",
            "sources":[
                "src/base.cpp",
                "src/base_js.cpp",
                "src/base_weak.cpp",
                "src/fonts/vector.c",
                "src/fonts/vertex-buffer.c",
                "src/fonts/vertex-attribute.c",
                "src/fonts/texture-atlas.c",
                "src/fonts/texture-font.c",
                "src/fonts/utf8-utils.c",
                "src/fonts/distance-field.c",
                "src/fonts/edtaa3func.c",
                "src/fonts/shader.c",
                "src/fonts/mat4.c",
                "src/fonts.cpp",
                "src/images.cpp",
                "src/videos.cpp",
                "src/shaders.cpp",
                "src/renderer.cpp",
                "src/mathutils.cpp"
            ],
            "include_dirs": [
                "<!(node -e \"require('nan')\")",
                "src/",
                "src/fonts/",
                "src/images/"
            ],
            "cflags": [
                "-Wall",
            ],
            "cxxflags": [
                "-std=c++11"
            ],

            'conditions': [
                # macOS
                ['OS=="mac"', {
                    "include_dirs": [
                        " <!@(freetype-config --cflags)",
                        " <!@(pkg-config --cflags glfw3)"
                    ],
                    "libraries": [
                        " <!@(pkg-config --libs glfw3)",
                        '-framework OpenGL',
                        '-framework OpenCL',
                        '-framework IOKit',
                        '<!@(freetype-config --libs)',
                        '-ljpeg',
                        '-lpng',
                        '-lavcodec',
                        '-lavformat',
                        '-lswscale'
                    ],
                    "sources": [
                        "src/mac.cpp",
                    ],
                    "defines": [
                        "MAC",
                        "GLFW_NO_GLU",
                        "GLFW_INCLUDE_GL3",

                        # VAO not working
                        #"FREETYPE_GL_USE_VAO"
                    ],
                    "xcode_settings": {
                        "OTHER_CPLUSPLUSFLAGS": [
                            "-std=c++11",
                            "-stdlib=libc++"
                        ],
                        "OTHER_LDFLAGS": [
                            "-stdlib=libc++"
                        ],
                        "MACOSX_DEPLOYMENT_TARGET": "10.7"
                    }
                }],

                # Raspberry Pi
                ['OS=="linux"', {
					"conditions" : [
	                    ["target_arch=='arm'", {
		                "sources": [
                                	"src/ilclient/ilclient.c",
                                	"src/ilclient/ilcore.c",
		                	"src/rpi.cpp",
                                	"src/rpi_video.cpp"
		                    ],
		                "libraries": [
		                        "-L ../../../../../staging/usr/lib/",
                                	"-lbcm_host",
		                	"-lGLESv2",
		                        "-lEGL",
                                	"-lopenmaxil",
                                	"-lvcos",
                                	"-lvchiq_arm",
		                        '<!@(freetype-config --libs)',
                                	"-ljpeg",
                                	"-lpng",
                                	'-lavcodec',
                                	'-lavformat',
                                	'-lswscale'
		                    ],
		                    "defines": [
		                        "RPI"
		                    ],
		                    "include_dirs": [
					"../../../../../staging/usr/include",
					"../../../../../staging/usr/include/freetype2",
					"../../../../../staging/usr/include/IL",
		                        "../../../../../staging/usr/include/interface/vcos/pthreads",
		                        "../../../../../staging/usr/include/interface/vmcs_host/linux",
                                	"../../../../../staging/usr/include/interface/vchiq/",
		                        '<!@(freetype-config --cflags)'
		                    ],
                            "cflags": [
                                "-DHAVE_LIBOPENMAX=2",
                                "-DOMX",
                                "-DOMX_SKIP64BIT",
                                "-DUSE_EXTERNAL_OMX",
                                "-DHAVE_LIBBCM_HOST",
                                "-DUSE_EXTERNAL_LIBBCM_HOST",
                                "-DUSE_VCHIQ_ARM",
                                # get stack trace on ARM
                                "-funwind-tables",
                                "-rdynamic"
                            ]
		                }],

		                ["target_arch!='arm'", {
		                    "sources": [
		                        "src/mac.cpp"
		                    ],
		                    "libraries":[
		                        '<!@(freetype-config --libs)',
		                        "-lglfw",
                                "-ljpeg",
                                "-lpng"
		                    ],
		                    "defines": [
		                        "GL_GLEXT_PROTOTYPES",
		                        "LINUX"
		                    ],
		                    "include_dirs": [
		                        "../../../../../staging/usr/include/freetype2",
		                        "<!@(freetype-config --cflags)"
		                    ]
		                }]
		            ]
                }]
            ]
        },
        {
            "target_name": "action_after_build",
            "type": "none",
            "dependencies": [ "<(module_name)" ],
            "copies": [{
                "files": [ "<(PRODUCT_DIR)/<(module_name).node" ],
                "destination": "<(module_path)"
            }]
        }
    ]
}
