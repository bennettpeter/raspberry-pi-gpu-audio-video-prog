#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <OMX_Component.h>

#include <bcm_host.h>
#include <ilclient.h>

#define IMG "cimg0135.jpg"
//#define IMG  "jan.jpg";
//#define IMG "hype.jpg"
char *img_file = IMG;

void printState(OMX_HANDLETYPE handle)
{
    OMX_STATETYPE state;
    OMX_ERRORTYPE err;

    err = OMX_GetState(handle, &state);
    if (err != OMX_ErrorNone)
    {
        fprintf(stderr, "Error on getting state\n");
        exit(1);
    }
    switch (state)
    {
    case OMX_StateLoaded:
        printf("StateLoaded\n");
        break;
    case OMX_StateIdle:
        printf("StateIdle\n");
        break;
    case OMX_StateExecuting:
        printf("StateExecuting\n");
        break;
    case OMX_StatePause:
        printf("StatePause\n");
        break;
    case OMX_StateWaitForResources:
        printf("StateWait\n");
        break;
    case OMX_StateInvalid:
        printf("StateInvalid\n");
        break;
    default:
        printf("State unknown\n");
        break;
    }
}

char *err2str(int err)
{
    switch (err)
    {
    case OMX_ErrorInsufficientResources:
        return "OMX_ErrorInsufficientResources";
    case OMX_ErrorUndefined:
        return "OMX_ErrorUndefined";
    case OMX_ErrorInvalidComponentName:
        return "OMX_ErrorInvalidComponentName";
    case OMX_ErrorComponentNotFound:
        return "OMX_ErrorComponentNotFound";
    case OMX_ErrorInvalidComponent:
        return "OMX_ErrorInvalidComponent";
    case OMX_ErrorBadParameter:
        return "OMX_ErrorBadParameter";
    case OMX_ErrorNotImplemented:
        return "OMX_ErrorNotImplemented";
    case OMX_ErrorUnderflow:
        return "OMX_ErrorUnderflow";
    case OMX_ErrorOverflow:
        return "OMX_ErrorOverflow";
    case OMX_ErrorHardware:
        return "OMX_ErrorHardware";
    case OMX_ErrorInvalidState:
        return "OMX_ErrorInvalidState";
    case OMX_ErrorStreamCorrupt:
        return "OMX_ErrorStreamCorrupt";
    case OMX_ErrorPortsNotCompatible:
        return "OMX_ErrorPortsNotCompatible";
    case OMX_ErrorResourcesLost:
        return "OMX_ErrorResourcesLost";
    case OMX_ErrorNoMore:
        return "OMX_ErrorNoMore";
    case OMX_ErrorVersionMismatch:
        return "OMX_ErrorVersionMismatch";
    case OMX_ErrorNotReady:
        return "OMX_ErrorNotReady";
    case OMX_ErrorTimeout:
        return "OMX_ErrorTimeout";
    case OMX_ErrorSameState:
        return "OMX_ErrorSameState";
    case OMX_ErrorResourcesPreempted:
        return "OMX_ErrorResourcesPreempted";
    case OMX_ErrorPortUnresponsiveDuringAllocation:
        return "OMX_ErrorPortUnresponsiveDuringAllocation";
    case OMX_ErrorPortUnresponsiveDuringDeallocation:
        return "OMX_ErrorPortUnresponsiveDuringDeallocation";
    case OMX_ErrorPortUnresponsiveDuringStop:
        return "OMX_ErrorPortUnresponsiveDuringStop";
    case OMX_ErrorIncorrectStateTransition:
        return "OMX_ErrorIncorrectStateTransition";
    case OMX_ErrorIncorrectStateOperation:
        return "OMX_ErrorIncorrectStateOperation";
    case OMX_ErrorUnsupportedSetting:
        return "OMX_ErrorUnsupportedSetting";
    case OMX_ErrorUnsupportedIndex:
        return "OMX_ErrorUnsupportedIndex";
    case OMX_ErrorBadPortIndex:
        return "OMX_ErrorBadPortIndex";
    case OMX_ErrorPortUnpopulated:
        return "OMX_ErrorPortUnpopulated";
    case OMX_ErrorComponentSuspended:
        return "OMX_ErrorComponentSuspended";
    case OMX_ErrorDynamicResourcesUnavailable:
        return "OMX_ErrorDynamicResourcesUnavailable";
    case OMX_ErrorMbErrorsInFrame:
        return "OMX_ErrorMbErrorsInFrame";
    case OMX_ErrorFormatNotDetected:
        return "OMX_ErrorFormatNotDetected";
    case OMX_ErrorContentPipeOpenFailed:
        return "OMX_ErrorContentPipeOpenFailed";
    case OMX_ErrorContentPipeCreationFailed:
        return "OMX_ErrorContentPipeCreationFailed";
    case OMX_ErrorSeperateTablesUsed:
        return "OMX_ErrorSeperateTablesUsed";
    case OMX_ErrorTunnelingUnsupported:
        return "OMX_ErrorTunnelingUnsupported";
    default:
        return "unknown error";
    }
}

void print_port_info(OMX_HANDLETYPE handle, int portindex)
{

    OMX_PARAM_PORTDEFINITIONTYPE portdef;
    memset(&portdef, 0, sizeof(OMX_PARAM_PORTDEFINITIONTYPE));
    portdef.nSize = sizeof(OMX_PARAM_PORTDEFINITIONTYPE);
    portdef.nVersion.nVersion = OMX_VERSION;
    portdef.nPortIndex = portindex;

    OMX_GetParameter(handle,
                     OMX_IndexParamPortDefinition,
                     &portdef);
    char *domain;

    printf("Port %d\n", portdef.nPortIndex);
    if (portdef.eDir == OMX_DirInput)
    {
        printf("  is input port\n");
    }
    else
    {
        printf("  is output port\n");
    }

    switch (portdef.eDomain)
    {
    case OMX_PortDomainAudio:
        domain = "Audio";
        break;
    case OMX_PortDomainVideo:
        domain = "Video";
        break;
    case OMX_PortDomainImage:
        domain = "Image";
        break;
    case OMX_PortDomainOther:
        domain = "Other";
        break;
    }
    printf("  Domain is %s\n", domain);

    printf("  Buffer count %d\n", portdef.nBufferCountActual);
    printf("  Buffer minimum count %d\n", portdef.nBufferCountMin);
    printf("  Buffer size %d bytes\n", portdef.nBufferSize);
    printf("  Enabled %d\n", portdef.bEnabled);
    printf("  Populated %d\n", portdef.bPopulated);
}

void eos_callback(void *userdata, COMPONENT_T *comp, OMX_U32 data)
{
    fprintf(stderr, "Got eos event\n");
}

void error_callback(void *userdata, COMPONENT_T *comp, OMX_U32 data)
{
    fprintf(stderr, "OMX error %s\n", err2str(data));
}

int get_file_size(char *fname)
{
    struct stat st;

    if (stat(fname, &st) == -1)
    {
        perror("Stat'ing img file");
        return -1;
    }
    return (st.st_size);
}

unsigned int uWidth;
unsigned int uHeight;

unsigned int nBufferSize;

OMX_ERRORTYPE read_into_buffer_and_empty(FILE *fp,
                                         COMPONENT_T *component,
                                         OMX_BUFFERHEADERTYPE *buff_header,
                                         int *toread)
{
    OMX_ERRORTYPE r;

    int buff_size = buff_header->nAllocLen;
    int nread = fread(buff_header->pBuffer, 1, buff_size, fp);

    printf("Read %d\n", nread);

    buff_header->nFilledLen = nread;
    *toread -= nread;
    if (*toread <= 0)
    {
        printf("Setting EOS on input\n");
        buff_header->nFlags |= OMX_BUFFERFLAG_EOS;
    }
    r = OMX_EmptyThisBuffer(ilclient_get_handle(component),
                            buff_header);
    if (r != OMX_ErrorNone)
    {
        fprintf(stderr, "Empty buffer error %s\n",
                err2str(r));
    }
    return r;
}

static void set_image_decoder_input_format(COMPONENT_T *component)
{

    printf("Setting image decoder format\n");
    OMX_IMAGE_PARAM_PORTFORMATTYPE imagePortFormat;

    memset(&imagePortFormat, 0, sizeof(OMX_IMAGE_PARAM_PORTFORMATTYPE));
    imagePortFormat.nSize = sizeof(OMX_IMAGE_PARAM_PORTFORMATTYPE);
    imagePortFormat.nVersion.nVersion = OMX_VERSION;

    imagePortFormat.nPortIndex = 320;
    imagePortFormat.eCompressionFormat = OMX_IMAGE_CodingJPEG;
    OMX_SetParameter(ilclient_get_handle(component),
                     OMX_IndexParamImagePortFormat, &imagePortFormat);
}

void setup_decodeComponent(ILCLIENT_T *handle,
                           char *decodeComponentName,
                           COMPONENT_T **decodeComponent)
{
    int err;

    err = ilclient_create_component(handle,
                                    decodeComponent,
                                    decodeComponentName,
                                    ILCLIENT_DISABLE_ALL_PORTS |
                                        ILCLIENT_ENABLE_INPUT_BUFFERS |
                                        ILCLIENT_ENABLE_OUTPUT_BUFFERS);
    if (err == -1)
    {
        fprintf(stderr, "DecodeComponent create failed\n");
        exit(1);
    }
    printState(ilclient_get_handle(*decodeComponent));

    err = ilclient_change_component_state(*decodeComponent,
                                          OMX_StateIdle);
    if (err < 0)
    {
        fprintf(stderr, "Couldn't change state to Idle\n");
        exit(1);
    }
    printState(ilclient_get_handle(*decodeComponent));

    // must be before we enable buffers
    set_image_decoder_input_format(*decodeComponent);
}

void setup_renderComponent(ILCLIENT_T *handle,
                           char *renderComponentName,
                           COMPONENT_T **renderComponent)
{
    int err;

    err = ilclient_create_component(handle,
                                    renderComponent,
                                    renderComponentName,
                                    ILCLIENT_DISABLE_ALL_PORTS |
                                        ILCLIENT_ENABLE_INPUT_BUFFERS);
    if (err == -1)
    {
        fprintf(stderr, "RenderComponent create failed\n");
        exit(1);
    }
    printState(ilclient_get_handle(*renderComponent));

    err = ilclient_change_component_state(*renderComponent,
                                          OMX_StateIdle);
    if (err < 0)
    {
        fprintf(stderr, "Couldn't change state to Idle\n");
        exit(1);
    }
    printState(ilclient_get_handle(*renderComponent));
}

OMX_BUFFERHEADERTYPE *pOutputBufferHeader = NULL;
OMX_BUFFERHEADERTYPE **ppRenderInputBufferHeader;

int setup_shared_buffer_format(COMPONENT_T *decodeComponent,
                               COMPONENT_T *renderComponent)
{
    OMX_PARAM_PORTDEFINITIONTYPE portdef, rportdef;
    ;
    int ret;
    OMX_ERRORTYPE err;

    // need to setup the input for the render with the output of the
    // decoder
    portdef.nSize = sizeof(OMX_PARAM_PORTDEFINITIONTYPE);
    portdef.nVersion.nVersion = OMX_VERSION;
    portdef.nPortIndex = 321;
    OMX_GetParameter(ilclient_get_handle(decodeComponent),
                     OMX_IndexParamPortDefinition, &portdef);

    // Get default values of render
    rportdef.nSize = sizeof(OMX_PARAM_PORTDEFINITIONTYPE);
    rportdef.nVersion.nVersion = OMX_VERSION;
    rportdef.nPortIndex = 90;
    rportdef.nBufferSize = portdef.nBufferSize;
    nBufferSize = portdef.nBufferSize;

    err = OMX_GetParameter(ilclient_get_handle(renderComponent),
                           OMX_IndexParamPortDefinition, &rportdef);
    if (err != OMX_ErrorNone)
    {
        fprintf(stderr, "Error getting render port params %s\n", err2str(err));
        return err;
    }

    // tell render input what the decoder output will be providing
    //Copy some
    rportdef.format.video.nFrameWidth = portdef.format.image.nFrameWidth;
    rportdef.format.video.nFrameHeight = portdef.format.image.nFrameHeight;
    rportdef.format.video.nStride = portdef.format.image.nStride;
    rportdef.format.video.nSliceHeight = portdef.format.image.nSliceHeight;

    err = OMX_SetParameter(ilclient_get_handle(renderComponent),
                           OMX_IndexParamPortDefinition, &rportdef);
    if (err != OMX_ErrorNone)
    {
        fprintf(stderr, "Error setting render port params %s\n", err2str(err));
        return err;
    }
    else
    {
        printf("Render port params set up ok, buf size %d\n",
               portdef.nBufferSize);
    }

    return OMX_ErrorNone;
}

int use_buffer(COMPONENT_T *renderComponent,
               OMX_BUFFERHEADERTYPE *buff_header)
{
    int ret;
    OMX_PARAM_PORTDEFINITIONTYPE portdef;

    ppRenderInputBufferHeader =
        (OMX_BUFFERHEADERTYPE **)malloc(sizeof(void) *
                                        3);

    OMX_SendCommand(ilclient_get_handle(renderComponent),
                    OMX_CommandPortEnable, 90, NULL);
    /*
    ret=ilclient_wait_for_event(renderComponent,
                            OMX_EventCmdComplete,
                            OMX_CommandPortEnable, 1,
                            90, 1, ILCLIENT_EVENT_ERROR,  //PGB
                            5000);
    printf("wait for event status = %d\n",ret); //PGB
*/
    printState(ilclient_get_handle(renderComponent));

    print_port_info(ilclient_get_handle(renderComponent), 90); //PGB

    ret = OMX_UseBuffer(ilclient_get_handle(renderComponent),
                        &ppRenderInputBufferHeader[0],
                        90,
                        NULL,
                        nBufferSize,
                        buff_header->pBuffer);
    if (ret != OMX_ErrorNone)
    {
        fprintf(stderr, "Eror sharing buffer %s\n", err2str(ret));
        return ret;
    }
    else
    {
        printf("Sharing buffer ok\n");
    }

    ppRenderInputBufferHeader[0]->nAllocLen =
        buff_header->nAllocLen;

    int n;
    for (n = 1; n < 3; n++)
    {
        printState(ilclient_get_handle(renderComponent));
        ret = OMX_UseBuffer(ilclient_get_handle(renderComponent),
                            &ppRenderInputBufferHeader[n],
                            90,
                            NULL,
                            0,
                            NULL);
        if (ret != OMX_ErrorNone)
        {
            fprintf(stderr, "Eror sharing null buffer %s\n", err2str(ret));
            return ret;
        }
    }

    ilclient_enable_port(renderComponent, 90);

    ret = ilclient_change_component_state(renderComponent,
                                          OMX_StateExecuting);
    if (ret < 0)
    {
        fprintf(stderr, "Couldn't change render state to Executing\n");
        exit(1);
    }
    return 0;
}

int main(int argc, char **argv)
{

    int i;
    char *decodeComponentName;
    char *renderComponentName;
    int err;
    ILCLIENT_T *handle;
    COMPONENT_T *decodeComponent;
    COMPONENT_T *renderComponent;

    if (argc == 2)
    {
        img_file = argv[1];
    }
    FILE *fp = fopen(img_file, "r");
    int toread = get_file_size(img_file);

    OMX_BUFFERHEADERTYPE *buff_header;

    decodeComponentName = "image_decode";
    renderComponentName = "video_render";

    bcm_host_init();

    handle = ilclient_init();
    if (handle == NULL)
    {
        fprintf(stderr, "IL client init failed\n");
        exit(1);
    }

    if (OMX_Init() != OMX_ErrorNone)
    {
        ilclient_destroy(handle);
        fprintf(stderr, "OMX init failed\n");
        exit(1);
    }

    ilclient_set_error_callback(handle,
                                error_callback,
                                NULL);
    ilclient_set_eos_callback(handle,
                              eos_callback,
                              NULL);

    setup_decodeComponent(handle, decodeComponentName, &decodeComponent);
    setup_renderComponent(handle, renderComponentName, &renderComponent);
    // both components now in Idle state, no buffers, ports disabled

    // input port
    ilclient_enable_port_buffers(decodeComponent, 320,
                                 NULL, NULL, NULL);
    ilclient_enable_port(decodeComponent, 320);

    err = ilclient_change_component_state(decodeComponent,
                                          OMX_StateExecuting);
    if (err < 0)
    {
        fprintf(stderr, "Couldn't change decode state to Executing\n");
        exit(1);
    }
    printState(ilclient_get_handle(decodeComponent));

    // Read the first block so that the decodeComponent can get
    // the dimensions of the image and call port settings
    // changed on the output port to configure it
    buff_header =
        ilclient_get_input_buffer(decodeComponent,
                                  320,
                                  1 /* block */);
    if (buff_header != NULL)
    {
        read_into_buffer_and_empty(fp,
                                   decodeComponent,
                                   buff_header,
                                   &toread);

        // If all the file has been read in, then
        // we have to re-read this first block.
        // Broadcom bug?
        if (toread <= 0)
        {
            printf("Rewinding\n");
            // wind back to start and repeat
            fp = freopen(img_file, "r", fp);
            toread = get_file_size(img_file);
        }
    }

    // wait for first input block to set params for output port
    ilclient_wait_for_event(decodeComponent,
                            OMX_EventPortSettingsChanged,
                            321, 0, 0, 1,
                            ILCLIENT_EVENT_ERROR | ILCLIENT_PARAMETER_CHANGED,
                            10000);
    printf("Port settings changed\n");

    setup_shared_buffer_format(decodeComponent, renderComponent);

    ilclient_enable_port_buffers(decodeComponent, 321,
                                 NULL, NULL, NULL);
    ilclient_enable_port(decodeComponent, 321);

    // set decoder only to executing state
    err = ilclient_change_component_state(decodeComponent,
                                          OMX_StateExecuting);
    if (err < 0)
    {
        fprintf(stderr, "Couldn't change state to Executing\n");
        exit(1);
    }

    // now work through the file
    while (toread > 0)
    {
        OMX_ERRORTYPE r;

        // do we have a decode input buffer we can fill and empty?
        buff_header =
            ilclient_get_input_buffer(decodeComponent,
                                      320,
                                      1 /* block */);
        if (buff_header != NULL)
        {
            read_into_buffer_and_empty(fp,
                                       decodeComponent,
                                       buff_header,
                                       &toread);
        }

        // do we have an output buffer that has been filled?
        buff_header =
            ilclient_get_output_buffer(decodeComponent,
                                       321,
                                       0 /* no block */);
        if (buff_header != NULL)
        {
            printf("Got an output buffer length %d\n",
                   buff_header->nFilledLen);
            if (buff_header->nFlags & OMX_BUFFERFLAG_EOS)
            {
                printf("Got EOS\n");
            }
            if (pOutputBufferHeader == NULL && buff_header->nFilledLen > 0) // PGB
            {
                use_buffer(renderComponent, buff_header);
                pOutputBufferHeader = buff_header;
            }

            if (buff_header->nFilledLen > 0)
            {
                OMX_EmptyThisBuffer(ilclient_get_handle(renderComponent),
                                    buff_header);
            }

            OMX_FillThisBuffer(ilclient_get_handle(decodeComponent),
                               buff_header);
        }
    }

    int done = 0;
    while (!done)
    {
        printf("Getting last output buffers\n");
        buff_header =
            ilclient_get_output_buffer(decodeComponent,
                                       321,
                                       1 /* block */);
        printf("Got a final output buffer length %d\n",
               buff_header->nFilledLen);
        if (buff_header->nFlags & OMX_BUFFERFLAG_EOS)
        {
            printf("Got EOS\n");
            done = 1;
        }

        if (pOutputBufferHeader == NULL)
        {
            printf("Before use_buffer\n");
            use_buffer(renderComponent, buff_header);
            pOutputBufferHeader = buff_header;
        }

        ppRenderInputBufferHeader[0]->nFilledLen = buff_header->nFilledLen;
        printf("Before last Empty\n");
        OMX_EmptyThisBuffer(ilclient_get_handle(renderComponent),
                            ppRenderInputBufferHeader[0]);
        printf("After last Empty\n");
    }

    sleep(100);

    exit(0);
}
