# Phase 6 --- Windows Media Integration and Music Page

## Objective

Implement playback controls and now-playing information through Windows
media sessions. Spotify API is not a dependency for this phase.

## Windows media service

Use Windows Global System Media Transport Controls (GSMTC) APIs, likely
through the Windows SDK/WinRT projection supported by the selected .NET
target. Verify the exact API availability and project configuration
before coding.

Responsibilities:

-   Discover current media sessions.
-   Select the appropriate active session.
-   Handle session changes and application exit.
-   Retrieve title, artist, album, playback status, position, and
    duration when available.
-   Subscribe to metadata and playback-state changes.
-   Send normalized media updates to Pico.
-   Execute play/pause, next, and previous commands.
-   Handle missing metadata and unsupported operations.

Do not promise every media app exposes all fields. Keep provider
selection and no-session states explicit.

## Host-side abstraction

Define a provider interface so a Spotify-specific implementation can be
added later without changing the Pico protocol:

``` csharp
public interface IMediaProvider
{
    event EventHandler<MediaInfo?> MediaInfoChanged;
    Task PlayPauseAsync(CancellationToken cancellationToken);
    Task NextAsync(CancellationToken cancellationToken);
    Task PreviousAsync(CancellationToken cancellationToken);
}
```

Adjust signatures to project conventions and actual GSMTC API behavior.

## Device-facing media model

Define bounded metadata fields and explicit validity flags. For example:

-   title
-   artist
-   album
-   isPlaying
-   hasSession
-   positionMilliseconds (optional)
-   durationMilliseconds (optional)

Avoid sending large artwork in ordinary metadata updates.

## Artwork

Defer album artwork until basic controls and metadata work. If
implemented:

1.  Host retrieves artwork.
2.  Host converts it using the same documented RGB565 pipeline.
3.  Host transfers it as a file.
4.  Pico displays it using the bounded slideshow/image rendering path.

Do not create a second unrelated image renderer unless required.

## Pico music page

-   Play/pause control.
-   Previous/next controls.
-   Title and artist.
-   Optional album and progress display.
-   No-session/disconnected state.
-   Metadata truncation/scrolling behavior.
-   Touch controls send commands to host.
-   Music page remains visually stable when updates arrive rapidly.

## Spotify API future extension

Only add Spotify Web API if a concrete requirement cannot be satisfied
by GSMTC, such as playlist browsing or catalog search. Keep OAuth tokens
on the PC, never on the Pico. Do not make basic playback depend on
Spotify login or network availability.

## Acceptance criteria

-   Controls work with at least one supported Windows media session.
-   Metadata updates when track/session changes.
-   No-session and missing-field states are handled.
-   Pico does not require Spotify credentials or internet access.
-   Disconnecting USB does not break slideshow operation.
