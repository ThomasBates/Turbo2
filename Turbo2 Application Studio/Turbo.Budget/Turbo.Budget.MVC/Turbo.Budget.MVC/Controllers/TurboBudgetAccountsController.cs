using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;

using Turbo.Budget.MockRepository;
using Turbo.Budget.Types;

namespace Turbo.Budget.MVC.Controllers
{
    public class TurboBudgetAccountsController : Controller
    {
        ITurboBudgetRepository repository = new TurboBudgetMockRepository();

        //
        // GET: /TurboBudgetAccounts/

        public ActionResult Index()
        {
            return View(repository.Accounts);
        }

        public ActionResult Account(int id)
        {
            var account = repository.Accounts.Where<ITurboBudgetAccount>(a => a.ID == id).First();
            return View(account);
        }

        public ActionResult Transaction(int id)
        {
            var transaction = repository.Transactions.Where<ITurboBudgetTransaction>(t => t.ID == id).First();
            return View(transaction);
        }
    }
}
