USE Winlogon

DECLARE @bSendmail bit
SET @bSendmail = 1

DECLARE @SCARD_W_WRONG_CHV bigint
SET @SCARD_W_WRONG_CHV = -2146434965

DECLARE @crlf nvarchar(2)
SET @crlf = CHAR(13) + CHAR(10)

DECLARE @MessageBody nvarchar(4000)
SET @MessageBody = ""

DECLARE @Buffer nvarchar(256), @Number nvarchar(5), @Percent nvarchar(3)

DECLARE @Checkdate datetime
SET @Checkdate = DATEADD(day, -8, GETDATE())
--SET @Checkdate = DATEADD(day, -1, GETDATE())

--
-- Get number of card authentications
--
SELECT CARD
FROM AuthMonitor
WHERE TIMESTAMP > @Checkdate
AND CARD <> ""

DECLARE @NumCardAuth int
SET @NumCardAuth = @@ROWCOUNT

--
-- Get number of card failures
--
SELECT CARD
FROM AuthMonitor
WHERE TIMESTAMP > @Checkdate
AND CARD <> "" 
AND STATUS <> 0
AND STATUS <> @SCARD_W_WRONG_CHV

DECLARE @NumCardFailures int
SET @NumCardFailures = @@ROWCOUNT

--
-- Get number of TS card auth.
--
SELECT CARD
FROM AuthMonitor
WHERE TIMESTAMP > @Checkdate
AND CARD <> "" 
AND SESSION <> 0

DECLARE @NumTSCardAuth int
SET @NumTSCardAuth = @@ROWCOUNT

--
-- Get number of TS card failures
--
SELECT CARD
FROM AuthMonitor
WHERE TIMESTAMP > @Checkdate
AND CARD <> "" 
AND SESSION <> 0
AND STATUS <> 0

DECLARE @NumTSCardFailures int
SET @NumTSCardFailures = @@ROWCOUNT

--
-- Get number of authentications per CSP
--
CREATE TABLE #CspAuth
(
    CARD nvarchar(64),
    FAILURE int,
    NUMBER int
)

DECLARE @iCardHandle int, @stCard nvarchar(64)
SET @iCardHandle = 0
EXEC #GetCard @stCard OUTPUT, @iCardHandle OUTPUT

WHILE @stCard <> ""
BEGIN

    -- Get total number of card authentications
    SELECT CARD
    FROM AuthMonitor
    WHERE TIMESTAMP > @Checkdate
    AND CARD = @stCard 

    INSERT INTO #CspAuth VALUES (@stCard, 0, @@ROWCOUNT)

    -- Get number of failures per card
    SELECT CARD
    FROM AuthMonitor
    WHERE TIMESTAMP > @Checkdate
    AND CARD = @stCard 
    AND STATUS <> 0
    AND STATUS <> @SCARD_W_WRONG_CHV

    INSERT INTO #CspAuth VALUES (@stCard, 1, @@ROWCOUNT)
    EXEC #GetCard @stCard OUTPUT, @iCardHandle OUTPUT
END

--
-- Create the message for card authentications and failures
--
DECLARE @PerCardAuth AS nvarchar(1000)
SET @PerCardAuth = "Smart card authentications                Total |   Failures" + @crlf +
                    REPLICATE("-", 60) + @crlf

-- Total 
SET @Number = CAST(@NumCardAuth AS nvarchar(5))
EXEC master.dbo.xp_sprintf @Buffer OUTPUT, "   %-34s%5s%4s%%", "Total", @Number, "100"
SET @PerCardAuth = @PerCardAuth + @Buffer

-- Total failures
SET @Number = CAST(@NumCardFailures AS nvarchar(5))
IF @NumCardAuth <> 0 
    SET @Percent = CAST(@NumCardFailures * 100 / @NumCardAuth AS nvarchar(3))
ELSE
    SET @Percent = 0
EXEC master.dbo.xp_sprintf @Buffer OUTPUT, " | %5s%4s%%", @Number, @Percent
SET @PerCardAuth = @PerCardAuth + @Buffer + @crlf

-- TS
SET @Number = CAST(@NumTSCardAuth AS nvarchar(5))
SET @Percent = CAST(@NumTSCardAuth * 100 / @NumCardAuth AS nvarchar(3))
EXEC master.dbo.xp_sprintf @Buffer OUTPUT, "   %-34s%5s%4s%%", "TS Client", @Number, @Percent
SET @PerCardAuth = @PerCardAuth + @Buffer

-- TS failures
SET @Number = CAST(@NumTSCardFailures AS nvarchar(5))
IF @NumCardAuth <> 0 
    SET @Percent = CAST(@NumTSCardFailures * 100 / @NumTSCardAuth AS nvarchar(3))
ELSE
    SET @Percent = 0
EXEC master.dbo.xp_sprintf @Buffer OUTPUT, " | %5s%4s%%", @Number, @Percent
SET @PerCardAuth = @PerCardAuth + @Buffer + @crlf

DECLARE CardCursor CURSOR FOR
    SELECT CARD, FAILURE, NUMBER 
    FROM #CspAuth
    ORDER BY CARD ASC

DECLARE @Failure int, @NumAuth int
DECLARE @NumPerCardAuth int, @NumPerCardFailures int
SET @NumPerCardAuth = -1
SET @NumPerCardFailures = -1

OPEN CardCursor
FETCH NEXT FROM CardCursor
INTO @stCard, @Failure, @NumAuth

WHILE @@FETCH_STATUS = 0
BEGIN
    IF @Failure = 0
        SET @NumPerCardAuth = @NumAuth
    ELSE
        SET @NumPerCardFailures = @NumAuth

    IF @NumPerCardAuth <> -1 AND @NumPerCardFailures <> -1
	BEGIN	    
        -- Smart card
        SET @Number = CAST(@NumPerCardAuth AS nvarchar(5))
        SET @Percent = CAST(@NumPerCardAuth * 100 / @NumCardAuth AS nvarchar(3))
        EXEC master.dbo.xp_sprintf @Buffer OUTPUT, "   %-34s%5s%4s%%", @stCard, @Number, @Percent
        SET @PerCardAuth = @PerCardAuth + @Buffer

        -- Smart card failures
        SET @Number = CAST(@NumPerCardFailures AS nvarchar(5))
        IF @NumPerCardAuth <> 0
            SET @Percent = CAST(@NumPerCardFailures * 100 / @NumPerCardAuth AS nvarchar(3))
        ELSE
            SET @Percent = 0
        EXEC master.dbo.xp_sprintf @Buffer OUTPUT, " | %5s%4s%%", @Number, @Percent
        SET @PerCardAuth = @PerCardAuth + @Buffer + @crlf

	SET @NumPerCardAuth = -1
	SET @NumPerCardFailures = -1
	END

    FETCH NEXT FROM CardCursor
    INTO @stCard, @Failure, @NumAuth
END

CLOSE CardCursor
DEALLOCATE CardCursor

--
-- Get number of authentications per Reader
--
CREATE TABLE #ReaderAuth
(
    READER nvarchar(64),
    FAILURE int,
    NUMBER int
)

DECLARE @iReaderHandle int, @stReader nvarchar(64)
SET @iReaderHandle = 0
EXEC #GetReader @stReader OUTPUT, @iReaderHandle OUTPUT

WHILE @stReader <> ""
BEGIN

    -- Get total number of reader operations
    SELECT READER
    FROM AuthMonitor
    WHERE TIMESTAMP > @Checkdate
    AND READER LIKE @stReader + "%"

    INSERT INTO #ReaderAuth VALUES (@stReader, 0, @@ROWCOUNT)

    -- Get number of failures per reader
    SELECT READER
    FROM AuthMonitor
    WHERE TIMESTAMP > @Checkdate
    AND READER LIKE @stReader + "%"
    AND STATUS <> 0
    AND STATUS <> @SCARD_W_WRONG_CHV

    INSERT INTO #ReaderAuth VALUES (@stReader, 1, @@ROWCOUNT)
    EXEC #GetReader @stReader OUTPUT, @iReaderHandle OUTPUT
END
	
--
-- Create the message for Reader authentications and failures
--
DECLARE @PerReaderAuth AS nvarchar(1000)
SET @PerReaderAuth = "Reader operations                         Total |   Failures" + @crlf +
                      REPLICATE("-", 60) + @crlf

DECLARE ReaderCursor CURSOR FOR
    SELECT READER, FAILURE, NUMBER 
    FROM #ReaderAuth
    ORDER BY READER ASC

DECLARE @NumPerReaderAuth int, @NumPerReaderFailures int
SET @NumPerReaderAuth = -1
SET @NumPerReaderFailures = -1

OPEN ReaderCursor
FETCH NEXT FROM ReaderCursor
INTO @stReader, @Failure, @NumAuth

WHILE @@FETCH_STATUS = 0
BEGIN
    IF @Failure = 0
        SET @NumPerReaderAuth = @NumAuth
    ELSE
        SET @NumPerReaderFailures = @NumAuth

    IF @NumPerReaderAuth <> -1 AND @NumPerReaderFailures <> -1
	BEGIN	    
        -- Reader
        SET @Number = CAST(@NumPerReaderAuth AS nvarchar(5))
        SET @Percent = CAST(@NumPerReaderAuth * 100 / @NumCardAuth AS nvarchar(3))
        EXEC master.dbo.xp_sprintf @Buffer OUTPUT, "   %-34s%5s%4s%%", @stReader, @Number, @Percent
        SET @PerReaderAuth = @PerReaderAuth + @Buffer

        -- Reader failures
        SET @Number = CAST(@NumPerReaderFailures AS nvarchar(5))
        IF @NumPerReaderAuth <> 0
            SET @Percent = CAST(@NumPerReaderFailures * 100 / @NumPerReaderAuth AS nvarchar(3))
        ELSE
            SET @Percent = 0
        EXEC master.dbo.xp_sprintf @Buffer OUTPUT, " | %5s%4s%%", @Number, @Percent
        SET @PerReaderAuth = @PerReaderAuth + @Buffer + @crlf

	SET @NumPerReaderAuth = -1
	SET @NumPerReaderFailures = -1
	END

    FETCH NEXT FROM ReaderCursor
    INTO @stReader, @Failure, @NumAuth
END

CLOSE ReaderCursor
DEALLOCATE ReaderCursor

--
-- Get number of failures per failure type
--
DECLARE @Status bigint

DECLARE @FailureNumbers nvarchar(1400)
SET @FailureNumbers = "Failures by error code" + @crlf +
                      REPLICATE("-", 47) + @crlf

DECLARE FailureCursor CURSOR FOR
   SELECT DISTINCT STATUS 
   FROM AuthMonitor
   WHERE TIMESTAMP > @Checkdate
   AND CARD <> ""
   AND STATUS < 0
   AND STATUS <> @SCARD_W_WRONG_CHV

OPEN FailureCursor
FETCH NEXT FROM FailureCursor
INTO @Status

WHILE @@FETCH_STATUS = 0
BEGIN

    SELECT STATUS
    FROM AuthMonitor
    WHERE TIMESTAMP > @Checkdate
    AND CARD <> ""
    AND STATUS = @Status

    DECLARE @iNumFailures int
    SET @iNumFailures = @@ROWCOUNT
    SET @Number = CAST(@iNumFailures AS nvarchar(5))

    IF @NumCardFailures <> 0
        SET @Percent = CAST(@iNumFailures * 100 / @NumCardFailures AS nvarchar(3))
    ELSE
        SET @Percent = 0

    DECLARE @stError nvarchar(32), @stHex nvarchar(8)
    EXEC #Dec2Error @Status, @stHex OUTPUT, @stError OUTPUT

    EXEC master.dbo.xp_sprintf @Buffer OUTPUT, "   %-34s%5s %3s%%", @stError, @Number, @Percent

    SET @FailureNumbers = 
        @FailureNumbers + @Buffer + @crlf

    FETCH NEXT FROM FailureCursor
    INTO @Status
END

CLOSE FailureCursor
DEALLOCATE FailureCursor

--
-- Send mail
--

SET @MessageBody = @PerCardAuth + @crlf	+
                   @PerReaderAuth + @crlf + 
                   @FailureNumbers

IF @bSendmail <> 0
    EXEC master.dbo.xp_sendmail 
         @recipients = 'smcaft', 
         @message =  @MessageBody,
         @subject = 'Smart card self host report - authentication statistics'
ELSE
    PRINT @MessageBody

GO