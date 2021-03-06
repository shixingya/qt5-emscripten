/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the plugins of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QWINDOWSGLCONTEXT_H
#define QWINDOWSGLCONTEXT_H

#include "array.h"
#include "qtwindows_additional.h"

#include <qpa/qplatformopenglcontext.h>
#include <QtGui/QOpenGLContext>
#include <QtCore/QSharedPointer>

QT_BEGIN_NAMESPACE

class QDebug;

enum QWindowsGLFormatFlags
{
    QWindowsGLDirectRendering = 0x1,
    QWindowsGLOverlay = 0x2,
    QWindowsGLRenderToPixmap = 0x4,
    QWindowsGLAccumBuffer = 0x8
};

// Additional format information for Windows.
struct QWindowsOpenGLAdditionalFormat
{
    QWindowsOpenGLAdditionalFormat(unsigned formatFlagsIn = 0, unsigned pixmapDepthIn = 0, unsigned swapIntervalIn = -1) :
        formatFlags(formatFlagsIn), pixmapDepth(pixmapDepthIn), swapInterval(swapIntervalIn) {}
    unsigned formatFlags; // QWindowsGLFormatFlags.
    unsigned pixmapDepth; // for QWindowsGLRenderToPixmap
    int swapInterval;
};

// Per-window data for active OpenGL contexts.
struct QOpenGLContextData
{
    QOpenGLContextData(HGLRC r, HWND h, HDC d) : renderingContext(r), hwnd(h), hdc(d) {}
    QOpenGLContextData() : renderingContext(0), hwnd(0), hdc(0) {}

    HGLRC renderingContext;
    HWND hwnd;
    HDC hdc;
};

struct QWindowsOpenGLContextFormat
{
    QWindowsOpenGLContextFormat();
    static QWindowsOpenGLContextFormat current();
    void apply(QSurfaceFormat *format) const;

    QSurfaceFormat::OpenGLContextProfile profile;
    int version; //! majorVersion<<8 + minorVersion
    QSurfaceFormat::FormatOptions options;
};

QDebug operator<<(QDebug d, const QWindowsOpenGLContextFormat &);

class QOpenGLStaticContext
{
    Q_DISABLE_COPY(QOpenGLStaticContext)
    QOpenGLStaticContext();
public:
    enum Extensions
    {
        SampleBuffers = 0x1
    };

    typedef bool
        (APIENTRY *WglGetPixelFormatAttribIVARB)
            (HDC hdc, int iPixelFormat, int iLayerPlane,
             uint nAttributes, const int *piAttributes, int *piValues);

    typedef bool
        (APIENTRY *WglChoosePixelFormatARB)(HDC hdc, const int *piAttribList,
            const float *pfAttribFList, uint nMaxFormats, int *piFormats,
            UINT *nNumFormats);

    typedef HGLRC
        (APIENTRY *WglCreateContextAttribsARB)(HDC, HGLRC, const int *);

    typedef BOOL
        (APIENTRY *WglSwapInternalExt)(int interval);
    typedef int
        (APIENTRY *WglGetSwapInternalExt)(void);

    bool hasExtensions() const
        { return wglGetPixelFormatAttribIVARB && wglChoosePixelFormatARB && wglCreateContextAttribsARB; }

    static QOpenGLStaticContext *create();
    static QByteArray getGlString(unsigned int which);

    const QByteArray vendor;
    const QByteArray renderer;
    const QByteArray extensionNames;
    unsigned extensions;
    const QWindowsOpenGLContextFormat defaultFormat;

    WglGetPixelFormatAttribIVARB wglGetPixelFormatAttribIVARB;
    WglChoosePixelFormatARB wglChoosePixelFormatARB;
    WglCreateContextAttribsARB wglCreateContextAttribsARB;
    WglSwapInternalExt wglSwapInternalExt;
    WglGetSwapInternalExt wglGetSwapInternalExt;
};

QDebug operator<<(QDebug d, const QOpenGLStaticContext &);

class QWindowsGLContext : public QPlatformOpenGLContext
{
public:
    typedef QSharedPointer<QOpenGLStaticContext> QOpenGLStaticContextPtr;

    explicit QWindowsGLContext(const QOpenGLStaticContextPtr &staticContext,
                               QOpenGLContext *context);
    virtual ~QWindowsGLContext();
    bool isSharing() const                { return m_context->shareHandle(); }
    bool isValid() const                  { return m_renderingContext; }
    virtual QSurfaceFormat format() const { return m_obtainedFormat; }

    virtual void swapBuffers(QPlatformSurface *surface);

    virtual bool makeCurrent(QPlatformSurface *surface);
    virtual void doneCurrent();

    typedef void (*GL_Proc) ();

    virtual GL_Proc getProcAddress(const QByteArray &procName);

    HGLRC renderingContext() const        { return m_renderingContext; }

private:
    inline void releaseDCs();

    const QOpenGLStaticContextPtr m_staticContext;
    QOpenGLContext *m_context;
    QSurfaceFormat m_obtainedFormat;
    HGLRC m_renderingContext;
    Array<QOpenGLContextData> m_windowContexts;
    PIXELFORMATDESCRIPTOR m_obtainedPixelFormatDescriptor;
    int m_pixelFormat;
    bool m_extensionsUsed;
};

QT_END_NAMESPACE

#endif // QWINDOWSGLCONTEXT_H
