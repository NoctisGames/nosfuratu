using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using NosFURatuComp;

namespace NosFURatu
{
    public partial class MainPage : PhoneApplicationPage
    {
        private Direct3DInterop m_d3dInterop = new Direct3DInterop();
        private object m_d3dContentProvider;

        public MainPage()
        {
            InitializeComponent();
        }

        protected override void OnNavigatedTo(System.Windows.Navigation.NavigationEventArgs e)
        {
            base.OnNavigatedTo(e);
        }

        protected override void OnNavigatedFrom(NavigationEventArgs e)
        {
            base.OnNavigatedFrom(e);
        }

        protected override void OnBackKeyPress(System.ComponentModel.CancelEventArgs e)
        {
            base.OnBackKeyPress(e);

            if (m_d3dInterop.onBackPressed())
            {
                e.Cancel = true;
            }
        }

        private void DrawingSurface_Loaded(object sender, RoutedEventArgs e)
        {
            // Set window bounds in dips
            m_d3dInterop.WindowBounds = new Windows.Foundation.Size((float)DrawingSurface.ActualWidth, (float)DrawingSurface.ActualHeight);

            // Set native resolution in pixels
            m_d3dInterop.NativeResolution = new Windows.Foundation.Size((float)Math.Floor(DrawingSurface.ActualWidth * Application.Current.Host.Content.ScaleFactor / 100.0f + 0.5f), (float)Math.Floor(DrawingSurface.ActualHeight * Application.Current.Host.Content.ScaleFactor / 100.0f + 0.5f));

            // Set render resolution to the full native resolution
            m_d3dInterop.RenderResolution = m_d3dInterop.NativeResolution;

            if (m_d3dContentProvider == null)
            {
                m_d3dContentProvider = m_d3dInterop.CreateContentProvider();
            }

            // Hook-up native component to DrawingSurface
            DrawingSurface.SetContentProvider(m_d3dContentProvider);
            DrawingSurface.SetManipulationHandler(m_d3dInterop);

            m_d3dInterop.setWinRtCallback(new WinRtCallback(ProcessCallback));
        }

        private void ProcessCallback(String command, String param)
        {
            if (command.Equals("EXIT"))
            {
                Deployment.Current.Dispatcher.BeginInvoke(() =>
                {
                    NavigationService.GoBack();
                });
            }
        }
    }
}