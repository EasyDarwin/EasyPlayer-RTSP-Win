using EasyPlayerNetSDK;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Forms;

namespace Client
{
    public partial class PlayerForm : Form
    {
        private PlayerSdk.MediaSourceCallBack callBack = null;
        private bool isInit = false;
        private int channelID = -1;
        private bool isTCP = false;
        private bool isHardEncode = false;

        public PlayerForm()
        {
            InitializeComponent();
        }

        /// <summary>
        /// 播放.
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="EventArgs"/> instance containing the event data.</param>
        private void PlayerBtn_Click(object sender, EventArgs e)
        {
            var sss = this.panel1.ClientRectangle;
            var isPlay = (sender as Button).Text == "播放";
            if (isPlay)
            {
                string RTSPStreamURI = "rtsp://127.0.0.1:9555/ch1";
                //string RTSPStreamURI = this.StreamURI.Text ?? "rtsp://admin:a1234567@192.168.90.32:554/h264/ch1/main/av_stream";
                channelID = PlayerSdk.EasyPlayer_OpenStream(RTSPStreamURI, this.panel1.Handle, PlayerSdk.RENDER_FORMAT.DISPLAY_FORMAT_RGB24_GDI, isTCP ? 1 : 0, "", "", IntPtr.Zero, callBack, isHardEncode);
                if (channelID > 0)
                    this.PlayerBtn.Text = "停止";
            }
            else
            {
                int ret = PlayerSdk.EasyPlayer_CloseStream(channelID);
                if (ret == 0)
                    this.PlayerBtn.Text = "播放";
            }
        }

        /// <summary>
        /// 截图.
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="EventArgs"/> instance containing the event data.</param>
        private void Snop_MenuItem_Click(object sender, EventArgs e)
        {
            if (channelID <= 0)
                return;
            int ret = PlayerSdk.EasyPlayer_PicShot(channelID);
        }

        /// <summary>
        /// 视频录制.
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="EventArgs"/> instance containing the event data.</param>
        private void Recode_MenuItem_Click(object sender, EventArgs e)
        {
            if (channelID <= 0)
                return;

            var checkState = (sender as ToolStripMenuItem).CheckState;
            if (checkState == CheckState.Unchecked)
            {
                int ret = PlayerSdk.EasyPlayer_StartManuRecording(channelID);
                (sender as ToolStripMenuItem).CheckState = CheckState.Checked;
            }
            if (checkState == CheckState.Checked)
            {
                int ret = PlayerSdk.EasyPlayer_StopManuRecording(channelID);
                (sender as ToolStripMenuItem).CheckState = CheckState.Unchecked;
            }
        }

        /// <summary>
        /// OSD信息显示.
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="EventArgs"/> instance containing the event data.</param>
        private void OSDShow_MenuItem_Click(object sender, EventArgs e)
        {
            if (channelID <= 0)
                return;

            var checkState = (sender as ToolStripMenuItem).CheckState;
            if (checkState == CheckState.Unchecked)
            {
                int ret = PlayerSdk.EasyPlayer_ShowStatisticalInfo(channelID, 1);
                (sender as ToolStripMenuItem).CheckState = CheckState.Checked;
            }
            if (checkState == CheckState.Checked)
            {
                int ret = PlayerSdk.EasyPlayer_ShowStatisticalInfo(channelID, 0);
                (sender as ToolStripMenuItem).CheckState = CheckState.Unchecked;
            }
        }

        /// <summary>
        /// 视频缓冲区设置.
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="EventArgs"/> instance containing the event data.</param>
        private void CacheFream_ValueChanged(object sender, EventArgs e)
        {
            if (channelID <= 0)
                return;

            var cache = (sender as TrackBar).Value;
            int ret = PlayerSdk.EasyPlayer_SetFrameCache(channelID, cache);
        }

        /// <summary>
        /// 数据流回调
        /// </summary>
        /// <param name="_channelId">The _channel identifier.</param>
        /// <param name="_channelPtr">The _channel PTR.</param>
        /// <param name="_frameType">Type of the _frame.</param>
        /// <param name="pBuf">The p buf.</param>
        /// <param name="_frameInfo">The _frame information.</param>
        /// <returns>System.Int32.</returns>
        private int MediaCallback(int _channelId, IntPtr _channelPtr, int _frameType, IntPtr pBuf, ref PlayerSdk.EASY_FRAME_INFO _frameInfo)
        {
            return 0;
        }

        /// <summary>
        /// 比例显示(仅限于软解码).
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="EventArgs"/> instance containing the event data.</param>
        private void FullWindos_MenuItem_Click(object sender, EventArgs e)
        {
            if (channelID > 0)
            {
                var checkState = (sender as ToolStripMenuItem).CheckState;
                if (checkState == CheckState.Unchecked)
                {
                    int ret = PlayerSdk.EasyPlayer_SetShownToScale(channelID, 1);
                    (sender as ToolStripMenuItem).CheckState = CheckState.Checked;
                }
                if (checkState == CheckState.Checked)
                {
                    int ret = PlayerSdk.EasyPlayer_SetShownToScale(channelID, 0);
                    (sender as ToolStripMenuItem).CheckState = CheckState.Unchecked;
                }
            }
        }

        /// <summary>
        /// 关键帧播放
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="EventArgs"/> instance containing the event data.</param>
        private void KeyFreamDecode_MenuItem_Click(object sender, EventArgs e)
        {
            if (channelID <= 0)
                return;

            var checkState = (sender as ToolStripMenuItem).CheckState;
            if (checkState == CheckState.Unchecked)
            {
                int ret = PlayerSdk.EasyPlayer_SetDecodeType(channelID, 1);
                (sender as ToolStripMenuItem).CheckState = CheckState.Checked;
            }
            if (checkState == CheckState.Checked)
            {
                int ret = PlayerSdk.EasyPlayer_SetShownToScale(channelID, 0);
                (sender as ToolStripMenuItem).CheckState = CheckState.Unchecked;
            }

        }

        /// <summary>
        /// 局部渲染
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void RenderRect_MenuItem_Click(object sender, EventArgs e)
        {
            if (channelID <= 0)
                return;

            var panelSize = this.panel1.ClientSize;
            int ret = PlayerSdk.EasyPlayer_SetRenderRect(channelID, new Rect { X = 0, Y = 0, Width = panelSize.Width / 2, Height = panelSize.Height / 2 });
            (sender as ToolStripMenuItem).CheckState = CheckState.Checked;

        }

        /// <summary>
        /// 播放音频
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void PlaySound_MenuItem_Click(object sender, EventArgs e)
        {
            if (channelID <= 0)
                return;

            var checkState = (sender as ToolStripMenuItem).CheckState;
            if (checkState == CheckState.Unchecked)
            {
                int ret = PlayerSdk.EasyPlayer_PlaySound(channelID);
                (sender as ToolStripMenuItem).CheckState = CheckState.Checked;
            }
            if (checkState == CheckState.Checked)
            {
                int ret = PlayerSdk.EasyPlayer_StopSound();
                (sender as ToolStripMenuItem).CheckState = CheckState.Unchecked;
            }
        }
        
        private void PlayerForm_FormClosing(object sender, System.Windows.Forms.FormClosingEventArgs e)
        {
            if (isInit)
                PlayerSdk.EasyPlayer_Release();
        }

        private void PlayerForm_Load(object sender, System.EventArgs e)
        {
            if (PlayerSdk.EasyPlayer_Init() == 0)
                isInit = true;
            callBack = new PlayerSdk.MediaSourceCallBack(MediaCallback);

            isTCP = rtpoverType.CheckState == CheckState.Checked;
            isHardEncode = HardDecode.CheckState == CheckState.Checked;
            this.RightToLeft = RightToLeft.Inherit;
        }
    }
}
