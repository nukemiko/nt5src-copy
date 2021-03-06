if exists (select * from sysobjects where id = object_id(N'[dbo].[GetQryOP]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[GetQryOP]
GO

if exists (select * from sysobjects where id = object_id(N'[dbo].[GetRCIncidentDetail]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[GetRCIncidentDetail]
GO

if exists (select * from sysobjects where id = object_id(N'[dbo].[GetRCIncidentsQryFieldSE]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[GetRCIncidentsQryFieldSE]
GO

if exists (select * from sysobjects where id = object_id(N'[dbo].[RunRCIncidentQueryCust]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[RunRCIncidentQueryCust]
GO

if exists (select * from sysobjects where id = object_id(N'[dbo].[RunRCIncidentQuerySE]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[RunRCIncidentQuerySE]
GO

if exists (select * from sysobjects where id = object_id(N'[dbo].[sp_AddRCIncident]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[sp_AddRCIncident]
GO

if exists (select * from sysobjects where id = object_id(N'[dbo].[sp_UpdateRCIncident]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[sp_UpdateRCIncident]
GO

SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  OFF 
GO

CREATE PROC dbo.GetQryOP
As
 Select * From TblQryOP

GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO


CREATE Proc dbo.GetRCIncidentDetail
	@iIncidentID int
As
	Select sUserName, sDescription, sFile
		from TblRCIncidents
		where iIncidentID = @iIncidentID
 
return @@ERROR


GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO



CREATE Proc dbo.GetRCIncidentsQryFieldSE
As
	Select * from TblRCIncidentsQryFieldSE
	Order by sFieldName





GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO


CREATE Proc dbo.RunRCIncidentQueryCust
		-- @sFrom nvarchar (1000),
		-- We probably dont need this sFrom, since we select from
		-- a single table.
		@sWhere nvarchar (2000),
		@iStart int, -- 1 ~ N-1
		@iGet int,
		@iTotal int OUTPUT,
		@iRet int OUTPUT
As
Begin
	Declare @sExec nvarchar (3000)
	Declare @i int, @j int, @iIncd int
	Declare @ErrorStatus int

	Select @iRet = 0
	Select @ErrorStatus = 0
	Set @sWhere = N'(' + @sWhere + N')'
	
	Create Table #tblTmp (iIncd int)

	Set NoCount ON
	Set @sExec = N'Insert #tblTmp (iIncd) Select Distinct iIncidentID 
				From ' + N'TblRCIncidents' + N' Where ' + 
				@sWhere
	Exec (@sExec)

	Select @iTotal = Count (*) from #tblTmp
	If @iTotal IS NULL SET @iTotal = 0
	If (@iStart > @iTotal) OR (@iStart + @iGet > @iTotal)
	Begin
		If (@iStart > @iTotal) Goto lblExit
		Select @iGet = @iTotal - @iStart + 1
	End
	Set @i = @iStart + @iGet - 1
	Declare tbl_Cursor CURSOR LOCAL READ_ONLY For
		Select * from #tblTmp
	Open tbl_Cursor
	While @iStart > 1
	Begin 
		Fetch Next From tbl_Cursor into @iIncd
		Set @iStart = @iStart - 1
	End
	Set @iRet = @iGet
	Create Table #tmp1 (iIncd int)
	While @iGet > 0
	Begin
		Fetch Next From tbl_Cursor into @iIncd
		Insert into #tmp1 Values (@iIncd)
		Set @iGet = @iGet - 1
		If @@FETCH_STATUS <> 0 Break
	End
	Set @iRet = @iRet - @iGet

	Select a.iIncidentID, dtUploadDate, sDescription, iStatus
		From TblRCIncidents a, #tmp1 b
		Where a.iIncidentID = b.iIncd
	
	Drop Table #tmp1
	Close tbl_Cursor
	Deallocate tbl_Cursor

lblExit:
	Set NoCount Off
	Drop Table #tblTmp
	Return @ErrorStatus
End


GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO



CREATE Proc dbo.RunRCIncidentQuerySE
		-- @sFrom nvarchar (1000),
		-- We probably dont need this sFrom, since we select from
		-- a single table.
		@sWhere nvarchar (2000),
		@iStart int, -- 1 ~ N-1
		@iGet int,
		@iTotal int OUTPUT,
		@iRet int OUTPUT
As
Begin
	Declare @sExec nvarchar (3000)
	Declare @i int, @j int, @iIncd int
	Declare @ErrorStatus int

	Select @iRet = 0
	Select @ErrorStatus = 0
	Set @sWhere = N'(' + @sWhere + N')'
	
	Create Table #tblTmp (iIncd int)

	Set NoCount ON
	Set @sExec = N'Insert #tblTmp (iIncd) Select Distinct iIncidentID 
				From ' + N'TblRCIncidents' + N' Where ' + 
				@sWhere
	Exec (@sExec)

	Select @iTotal = Count (*) from #tblTmp
	If @iTotal IS NULL SET @iTotal = 0
	If (@iStart > @iTotal) OR (@iStart + @iGet > @iTotal)
	Begin
		If (@iStart > @iTotal) Goto lblExit
		Select @iGet = @iTotal - @iStart + 1
	End
	Set @i = @iStart + @iGet - 1
	Declare tbl_Cursor CURSOR LOCAL READ_ONLY For
		Select * from #tblTmp
	Open tbl_Cursor
	While @iStart > 1
	Begin 
		Fetch Next From tbl_Cursor into @iIncd
		Set @iStart = @iStart - 1
	End
	Set @iRet = @iGet
	Create Table #tmp1 (iIncd int)
	While @iGet > 0
	Begin
		Fetch Next From tbl_Cursor into @iIncd
		Insert into #tmp1 Values (@iIncd)
		Set @iGet = @iGet - 1
		If @@FETCH_STATUS <> 0 Break
	End
	Set @iRet = @iRet - @iGet

	Select a.iIncidentID, sUserName, dtUploadDate, iMemberLow, 
					iMemberHigh, iStatus
		From TblRCIncidents a, #tmp1 b
		Where a.iIncidentID = b.iIncd
	
	Drop Table #tmp1
	Close tbl_Cursor
	Deallocate tbl_Cursor

lblExit:
	Set NoCount Off
	Drop Table #tblTmp
	Return @ErrorStatus
End
	
	
	








GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

CREATE Proc sp_AddRCIncident 
	@sUserName nvarchar(200),
	@sDescription nvarchar(1000),
	@iIndex int OUTPUT,
	@iError int OUTPUT
As
	Insert TblRCIncidents
	(sUserName, sDescription)
	Values
	(@sUserName, @sDescription);
	
	Select @iIndex = max (iIncidentID) from TblRCIncidents;
	Set @iError = @@ERROR;






GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

CREATE Proc sp_UpdateRCIncident
	@sFile nvarchar (1000),
	@iIndex int,
	@iError int OUTPUT
As
	Update TblRCIncidents
	Set sFile = @sFile
	Where iIncidentID = @iIndex;

	Set @iError = @@ERROR;


GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

